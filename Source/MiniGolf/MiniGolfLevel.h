// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MiniGolfLevel.generated.h"

/**
 * 
 */
UCLASS()
class MINIGOLF_API AMiniGolfLevel : public ALevelScriptActor
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly)
	int NumCoins;

	void OnBallHit();

	UPROPERTY(BlueprintReadOnly)
	int NumHits;
};
