// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniGolfBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"
#include "MiniGolf/MiniGolfLevel.h"

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
	JumpImpulse = 350000.0f;

	bCanHit = false;
}

//=================================================================================
void AMiniGolfBall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bCanHit)
		DrawDebugLine(GetWorld(), Ball->GetComponentLocation() + 0.1f * m_ForwardVector, Ball->GetComponentLocation() + 100.1f * m_ForwardVector, FColor::Red, false, -1.f, 0U, 5.f);

	FRigidBodyState state;
	if (Ball->GetRigidBodyState(state))
	{
		const auto timeUntilAllowHit = GetWorldTimerManager().GetTimerRemaining(m_SinceLastHit);
		if (!bCanHit && timeUntilAllowHit <= 0.0f && state.LinVel.Size() < 1e-4) {
			bCanHit = true;
		}
	}
}

//=================================================================================
void AMiniGolfBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("Turn", this, &AMiniGolfBall::TurnRight);

	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &AMiniGolfBall::Hit);
}

//=================================================================================
void AMiniGolfBall::BeginPlay()
{
	Super::BeginPlay();
	bCanHit = false;
	GetWorldTimerManager().SetTimer(m_SinceLastHit, 0.5f, false, 0.5f);
}

//=================================================================================
void AMiniGolfBall::TurnRight(float Val)
{
	const FRotator rotator(0.f, Val, 0.f);
	m_ForwardVector = rotator.RotateVector(m_ForwardVector);

	SpringArm->SetRelativeRotation(m_ForwardVector.Rotation().Add(-45.f, 0.f, 0.f));
}

//=================================================================================
void AMiniGolfBall::Hit()
{
	if (bCanHit)
	{
		const FVector Impulse = m_ForwardVector * JumpImpulse;
		Ball->AddImpulse(Impulse);

		if (auto* level = Cast< AMiniGolfLevel>(GetLevel())) {
			level->OnBallHit();
		}
		bCanHit = false;
		GetWorldTimerManager().SetTimer(m_SinceLastHit, 0.5f, false);
	}
}

//=================================================================================
void AMiniGolfBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	//bCanJump = true;
}