// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FMODEvent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MiniGolfBall.generated.h"

UCLASS(config=Game)
class AMiniGolfBall : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	const static float s_ChargeTime;

public:
	AMiniGolfBall();

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditAnywhere, Category=Ball)
	float PokeStrength;


	//=================================================================================
	void Tick(float DeltaSeconds) override;

protected:

	/** Charge for a hit **/
	void Charge();

	/** Handle ball hit action **/
	void Hit();

	void TurnRight(float Val);
	void Elevate(float Val);

	// AActor interface
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End of AActor interface

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	bool IsInCourse() const;

	void StopBall();

	FVector m_ForwardVector;

	/** Ball can be hit only when it is stationary **/
	UPROPERTY(BlueprintReadOnly)
	bool bCanHit;

	FTimerHandle m_SinceLastHit;
	FTimerHandle m_ChargingTime;

	FTimerHandle m_SinceSlowDown;

	//=================================================================================
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	float GetCurrentPower();

	UFUNCTION(BlueprintCallable)
	FVector GetCurrentDirection();

	UFUNCTION(BlueprintCallable)
	float GetMaxPower();
public:	
	UPROPERTY(EditAnywhere)
	UFMODEvent* m_Outside;
	UPROPERTY(EditAnywhere)
	UFMODEvent* m_HitWall;
	UPROPERTY(EditAnywhere)
	UFMODEvent* m_HitBall;

	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
