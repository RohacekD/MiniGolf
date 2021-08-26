// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniGolfBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MiniGolf/MGGameInstance.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"
#include "MiniGolf/MiniGolfLevel.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"

const float AMiniGolfBall::s_ChargeTime = 2.f;
const static float s_TimeSlowedDown = 1.f;

AMiniGolfBall::AMiniGolfBall()
	: m_ForwardVector(1.0f, 0.0f, 0.0f)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(2.f);
	Ball->SetLinearDamping(1.f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true); // Rotation of the ball should not affect rotation of boom
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Set up forces
	PokeStrength = 350000.0f;

	bCanHit = false;
}

//=================================================================================
void AMiniGolfBall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Cast<UMGGameInstance>(GetGameInstance())->IsPlayerControlled())
	{
		return;
	}
	FRigidBodyState state;
	if (Ball->GetRigidBodyState(state))
	{
		const auto timeUntilAllowHit = GetWorldTimerManager().GetTimerRemaining(m_SinceLastHit);
		const auto currentSpeed = state.LinVel.Size();
		if (!bCanHit && timeUntilAllowHit <= 0.0f) {
			if (currentSpeed < 0.1) // speed to small
			{
				StopBall();
			}
			else
			{
				// avoid endless waits
				if (currentSpeed < 50)
				{
					// enough time passed by
					const auto timeInSlowDownRemains = GetWorldTimerManager().GetTimerRemaining(m_SinceSlowDown);
					if (timeInSlowDownRemains == 0.f)
					{
						// ok, we waited long enough, time to sleep
						StopBall();
					}
					if (timeInSlowDownRemains < 0.f)
					{
						// start timer 
						GetWorldTimerManager().SetTimer(m_SinceSlowDown, s_TimeSlowedDown, false, s_TimeSlowedDown);
					}
				}
				else
				{
					// ball speeded up again
					GetWorldTimerManager().ClearTimer(m_SinceSlowDown);
				}
			}
		}
	}
}

//=================================================================================
void AMiniGolfBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("Turn", this, &AMiniGolfBall::TurnRight);
	PlayerInputComponent->BindAxis("Elevate", this, &AMiniGolfBall::Elevate);

	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &AMiniGolfBall::Charge);
	PlayerInputComponent->BindAction("Hit", IE_Released, this, &AMiniGolfBall::Hit);
}

//=================================================================================
bool AMiniGolfBall::IsInCourse() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhysicsVolume::StaticClass(), FoundActors);
	UE_LOG(LogTemp, Warning, TEXT("Volume, %d"), (FoundActors.Num()));
	for(const auto* volume : FoundActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Volume, %s"), *(volume->GetName()));
		const auto* physVolume = Cast<APhysicsVolume>(volume);
		if (physVolume && physVolume->IsOverlappingActor(this) && volume->ActorHasTag("CourseVolume"))
		{
			return true;
		}
	}
	return false;
}

//=================================================================================
void AMiniGolfBall::StopBall()
{
	Ball->PutRigidBodyToSleep();
	bCanHit = true;
	if (!IsInCourse())
	{
		if (auto* level = Cast< AMiniGolfLevel>(GetWorld()->GetLevelScriptActor())) {
			UFMODBlueprintStatics::PlayEvent2D(this, m_Outside, true);
			level->RespawnPlayer();
			bCanHit = false;
			GetWorldTimerManager().SetTimer(m_SinceLastHit, 0.5f, false, 0.5f);
		}
	}
}

//=================================================================================
void AMiniGolfBall::BeginPlay()
{
	Super::BeginPlay();
	bCanHit = false;
	GetWorldTimerManager().SetTimer(m_SinceLastHit, 0.5f, false, 0.5f);
}

//=================================================================================
float AMiniGolfBall::GetCurrentPower()
{
	if (!GetWorldTimerManager().IsTimerActive(m_ChargingTime))
	{
		return 0.f;
	}
	return GetWorldTimerManager().GetTimerElapsed(m_ChargingTime) / s_ChargeTime;
}

//=================================================================================
FVector AMiniGolfBall::GetCurrentDirection()
{
	return m_ForwardVector;
}

//=================================================================================
float AMiniGolfBall::GetMaxPower()
{
	return PokeStrength;
}

//=================================================================================
void AMiniGolfBall::TurnRight(float Val)
{
	const FRotator rotator(0.f, Val, 0.f);
	m_ForwardVector = rotator.RotateVector(m_ForwardVector);

	SpringArm->SetRelativeRotation(m_ForwardVector.Rotation().Add(-45.f, 0.f, 0.f));
}

//=================================================================================
void AMiniGolfBall::Elevate(float Val)
{
	// TODO needs quaternion cause of gimbal lock
	// const FRotator rotator(Val, 0.f, 0.f);
	// m_ForwardVector = rotator.RotateVector(m_ForwardVector);
	// m_ForwardVector.Z = FMath::Clamp(m_ForwardVector.Z, 0.f, .5f);
	// 
	// SpringArm->SetRelativeRotation(m_ForwardVector.Rotation().Add(-45.f, 0.f, 0.f));
}

//=================================================================================
void AMiniGolfBall::Charge()
{
	if(bCanHit)
		GetWorldTimerManager().SetTimer(m_ChargingTime, this, &AMiniGolfBall::Hit, s_ChargeTime, false, s_ChargeTime);
}

//=================================================================================
void AMiniGolfBall::Hit()
{
	if (bCanHit && GetWorldTimerManager().IsTimerActive(m_ChargingTime))
	{
		uint8 multiplier = 1;
		if (auto gameInstance = Cast<UMGGameInstance>(GetGameInstance()))
		{
			multiplier = gameInstance->iPokeMultiplier;
		}
		const FVector Impulse = GetCurrentDirection() * PokeStrength * GetCurrentPower() * multiplier;
		GetWorldTimerManager().ClearTimer(m_ChargingTime);
		Ball->AddImpulse(Impulse);

		if (auto* level = Cast< AMiniGolfLevel>(GetWorld()->GetLevelScriptActor())) {
			level->OnBallHit();
		}
		bCanHit = false;
		const auto transform = GetTransform();
		UFMODBlueprintStatics::PlayEventAtLocation(this, m_HitBall, transform, true);
		GetWorldTimerManager().SetTimer(m_SinceLastHit, 0.5f, false);
	}
}

//=================================================================================
void AMiniGolfBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	//bCanJump = true;
}