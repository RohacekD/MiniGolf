// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "MGGameInstance.generated.h"

UENUM(BlueprintType)
enum EMiniGolfLevels
{
	UEDPIE_0_GrassMill	UMETA(DisplayName = "GrassMill"),
	UEDPIE_0_WaterCreek	UMETA(DisplayName = "WaterCreek"),
};

/**
 *
 */
UCLASS(Blueprintable)
class MINIGOLF_API UMGGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMGGameInstance();
	~UMGGameInstance();

	UFUNCTION(BlueprintCallable)
		void LevelFinished(int score);

	UFUNCTION(BlueprintCallable)
	void CollectMoney(int amount = 1);

	UFUNCTION(BlueprintCallable)
	int GetMoney() const {return Money;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int NumPoints;
private:
	int Money;
};
