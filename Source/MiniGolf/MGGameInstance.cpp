// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "MiniGolf//MiniGolfLevel.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#define LOCTEXT_NAMESPACE "MyNamespace"

template<typename TEnum>
static FString GetEnumValueAsString(const FString& Name, TEnum Value) {
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr) return FString("Invalid");
	return enumPtr->GetNameByValue((int64)Value).ToString();
}

UMGGameInstance::UMGGameInstance()
	: m_bPlayerControlled(false)
{
}

UMGGameInstance::~UMGGameInstance() = default;

//=================================================================================
void UMGGameInstance::LevelFinished(int score)
{
	m_bPlayerControlled = false;
	ShowMenu();
	if (auto level = Cast<AMiniGolfLevel>(GetWorld()->GetLevelScriptActor())) {
		level->LevelFinished();
	}
	// UWorld* TheWorld = GetWorld();
	// 
	// FString CurrentLevel = TheWorld->GetMapName();
	// if (CurrentLevel == GetEnumValueAsString("EMiniGolfLevels", EMiniGolfLevels::UEDPIE_0_GrassMill))
	// {
	// 	UGameplayStatics::OpenLevel(GetWorld(), "WaterCreek");
	// }
	// else
	// {
	// 	UGameplayStatics::OpenLevel(GetWorld(), "GrassMill");
	// }
}

//=================================================================================
void UMGGameInstance::CollectMoney(int amount)
{
	// todo play sound
	Money += amount;
}

//=================================================================================
void UMGGameInstance::OpenLevel(EMiniGolfLevels level)
{
	switch (level)
	{
	case UEDPIE_0_GrassMill:
		UGameplayStatics::OpenLevel(GetWorld(), "GrassMill");
		break;
	case UEDPIE_0_WaterCreek:
		UGameplayStatics::OpenLevel(GetWorld(), "WaterCreek");
		break;
	default:
		break;
	}
	m_bPlayerControlled = true;
}

//=================================================================================
void UMGGameInstance::HideMenu()
{
	m_MainMenu->RemoveFromViewport();
}

//=================================================================================
void UMGGameInstance::ShowMenu()
{
	m_bPlayerControlled = false;
	m_MainMenu = CreateWidget<UUserWidget>(this, MainMenu);
	if (m_MainMenu)
		m_MainMenu->AddToViewport(9999);

	UpdateMoney();
}

//=================================================================================
void UMGGameInstance::OnStart()
{
	Super::OnStart();
	ShowMenu();
}

//=================================================================================
void UMGGameInstance::UpdateMoney()
{
	if (m_MainMenu)
	{
		auto* moneyTextBox = Cast<UTextBlock>(m_MainMenu->GetWidgetFromName("Money"));
		if (moneyTextBox)
		{
			moneyTextBox->SetText(FText::Format(LOCTEXT("Money", "Coins: {0}"), Money));
		}
	}
}
#undef LOCTEXT_NAMESPACE