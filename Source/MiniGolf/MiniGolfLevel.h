// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MiniGolfLevel.generated.h"

class AMiniGolfBall;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level info")
	int Par;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level info")
	FText LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> LevelGUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level info")
	AMiniGolfBall* m_Player;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level info")
	AActor* m_Hole;

	void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void PlayerControl();

	UFUNCTION(BlueprintCallable)
	void LevelFinished();

	UFUNCTION(BlueprintCallable)
	void UpdateDistance();
private:
	void UpdateHitsGUI();
	void UpdateLevelName();
	void UpdateCoinsText();

	UUserWidget* m_LevelGUI;


	FInputModeGameOnly inputModeGame;
	FInputModeUIOnly inputModeUI;
};
