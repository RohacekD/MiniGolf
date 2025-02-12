// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FMODEvent.h"
#include "Engine/GameInstance.h"
#include "FMODBlueprintStatics.h"
#include "fmod_studio.hpp"
#include "MGGameInstance.generated.h"


UENUM(BlueprintType)
enum EMiniGolfLevels
{
	UEDPIE_0_GrassMill	UMETA(DisplayName = "GrassMill"),
	UEDPIE_0_WaterCreek	UMETA(DisplayName = "WaterCreek"),
	UEDPIE_0_Void		UMETA(DisplayName = "Void"),
};

UENUM(BlueprintType)
enum class EMiniGolfBallColour : uint8
{
	Blue	UMETA(DisplayName = "Blue"),
	Pink	UMETA(DisplayName = "Pink"),
	Green	UMETA(DisplayName = "Green"),
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

	UFUNCTION(BlueprintCallable)
	void UpdateMoney();

	UFUNCTION(BlueprintCallable)
	void PayMoney(int money) {Money-=money;}

	UFUNCTION(BlueprintCallable)
	FColor GetBallColour() const;

	UPROPERTY(BlueprintReadWrite)
	EMiniGolfBallColour BallColour = EMiniGolfBallColour::Blue;

	UPROPERTY(BlueprintReadWrite)
	uint8 iPokeMultiplier = 1;

	UPROPERTY(BlueprintReadWrite)
	bool bMagneticHoles = false;

	UPROPERTY(BlueprintReadWrite)
	bool bTraceBall = false;

	UPROPERTY(BlueprintReadWrite)
	bool bLaserSightBall = false;



	UPROPERTY(BlueprintReadWrite)
	UFMODBank* m_MusicBank;
private:
	int Money;
	bool m_bPlayerControlled;

	FFMODEventInstance m_ActiveMusic;


	FTimerHandle m_PlayMenuMusic;
	void StartMenuMusic();

	UPROPERTY(EditAnywhere, Category = "Class Types")
	TSubclassOf<UUserWidget> MainMenu;
	UPROPERTY(EditAnywhere)
	UFMODEvent* m_MusicEvent;

	UUserWidget* m_MainMenu;
protected:
	//=================================================================================
	void OnStart() override;

};
