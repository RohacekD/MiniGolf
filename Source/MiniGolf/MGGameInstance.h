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
	void OpenLevel(EMiniGolfLevels level);

	UFUNCTION(BlueprintCallable)
	int GetMoney() const {return Money;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumPoints;

	UFUNCTION(BlueprintCallable)
	void HideMenu();

	UFUNCTION(BlueprintCallable)
	void ShowMenu();

	bool IsPlayerControlled() const { return m_bPlayerControlled; }

	void UpdateMoney();

private:
	int Money;
	bool m_bPlayerControlled;

	UPROPERTY(EditAnywhere, Category = "Class Types")
	TSubclassOf<UUserWidget> MainMenu;

	UUserWidget* m_MainMenu;
protected:
	//=================================================================================
	void OnStart() override;

};
