// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"
#include "Coin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoinCollected);

UCLASS()
class MINIGOLF_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

	UPROPERTY(BlueprintAssignable, Category= "EventDispatchers")
		FOnCoinCollected CoinCollected;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
