// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "Kismet/GameplayStatics.h"

template<typename TEnum>
static FString GetEnumValueAsString(const FString& Name, TEnum Value) {
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr) return FString("Invalid");
	return enumPtr->GetNameByValue((int64)Value).ToString();
}

UMGGameInstance::UMGGameInstance()
{

}

UMGGameInstance::~UMGGameInstance() = default;

//=================================================================================
void UMGGameInstance::LevelFinished(int score)
{
	UWorld* TheWorld = GetWorld();

	FString CurrentLevel = TheWorld->GetMapName();
	if (CurrentLevel == GetEnumValueAsString("EMiniGolfLevels", EMiniGolfLevels::UEDPIE_0_GrassMill))
	{
		UGameplayStatics::OpenLevel(GetWorld(), "WaterCreek");
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), "UEDPIE_0_GrassMill");
	}
}

//=================================================================================
void UMGGameInstance::CollectMoney()
{
	// todo play sound
	Money++;
}
