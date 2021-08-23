// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGolfLevel.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MiniGolf/MiniGolfBall.h"
#include "GameFramework/Pawn.h"
#include "MiniGolf/MGGameInstance.h"

#include "Coin.h"

#define LOCTEXT_NAMESPACE "MyNamespace"


//=================================================================================
void AMiniGolfLevel::BeginPlay()
{	
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoin::StaticClass(), FoundActors);
	for (auto* actor : FoundActors)
	{
		auto* coin = Cast<ACoin>(actor);
		//coin->CoinCollected.a;
	}
	NumCoins = FoundActors.Num();

	RespawnPlayer();

	if (Cast<UMGGameInstance>(GetGameInstance())->IsPlayerControlled())
	{
		PlayerControl();
	}
	else
	{
		ChangeControllsToUI();
	}
}

//=================================================================================
void AMiniGolfLevel::OnBallHit()
{
	NumHits++;
	UpdateHitsGUI();
}

//=================================================================================
void AMiniGolfLevel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateDistance();
}

//=================================================================================
void AMiniGolfLevel::PlayerControl()
{
	// FStringClassReference LevelGUIClassRef(TEXT("/Game/GUI/LevelGUI"));
	// const auto* LevelGUIClass = LevelGUIClassRef.TryLoadClass<UUserWidget>();

	m_LevelGUI = CreateWidget<UUserWidget>(GetGameInstance(), LevelGUI);
	UpdateHitsGUI();
	UpdateLevelName();
	UpdateCoinsText();
	const auto numControllers = GetWorld()->GetNumPlayerControllers();
	if (m_LevelGUI)
		m_LevelGUI->AddToViewport(numControllers);

	const auto controller = GetWorld()->GetFirstPlayerController();
	controller->bShowMouseCursor = false;
	controller->bEnableClickEvents = false;
	controller->bEnableMouseOverEvents = false;
	controller->SetInputMode(inputModeGame);


	if (IFMODStudioModule::IsAvailable())
	{
		FMOD::Studio::System* StudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
		if (StudioSystem)
		{
			m_ActiveMusic = UFMODBlueprintStatics::PlayEvent2D(this, m_LevelSoundrack, true);
			// Use it here
		}
	}
}

//=================================================================================
void AMiniGolfLevel::LevelFinished()
{
	m_LevelGUI->RemoveFromViewport();
	m_LevelGUI = nullptr;
	ChangeControllsToUI();

	UFMODBlueprintStatics::EventInstanceStop(m_ActiveMusic, true);
}

//=================================================================================
void AMiniGolfLevel::UpdateHitsGUI()
{
	if (m_LevelGUI)
	{
		auto* numHits = Cast<UTextBlock>(m_LevelGUI->GetWidgetFromName("NumHits"));
		if (numHits)
		{
			numHits->SetText(FText::Format(LOCTEXT("HitsGUI", "{0} {0}|plural(zero=hits,one=hit,other=hits) par {1}"), NumHits, Par));
		}
	}
}

//=================================================================================
void AMiniGolfLevel::UpdateLevelName()
{
	if (m_LevelGUI)
	{
		auto* numHits = Cast<UTextBlock>(m_LevelGUI->GetWidgetFromName("LevelName"));
		if (numHits)
		{
			numHits->SetText(LevelName);
		}
	}
}

//=================================================================================
void AMiniGolfLevel::UpdateCoinsText()
{
	if (m_LevelGUI)
	{
		auto* numHits = Cast<UTextBlock>(m_LevelGUI->GetWidgetFromName("CoinsCollected"));
		if (numHits)
		{
			numHits->SetText(FText::Format(LOCTEXT("CoinsCollected", "Coins {0}/{1}"), NumCollectedCoins, NumCoins));
		}
	}
}

//=================================================================================
void AMiniGolfLevel::ChangeControllsToUI()
{
	auto* firstPlayerController = GetWorld()->GetFirstPlayerController();
	if (firstPlayerController)
	{
		firstPlayerController->bShowMouseCursor = true;
		firstPlayerController->bEnableClickEvents = true;
		firstPlayerController->bEnableMouseOverEvents = true;
		firstPlayerController->SetInputMode(inputModeUI);

		firstPlayerController->UnPossess();
	}
}

//=================================================================================
void AMiniGolfLevel::RespawnPlayer()
{
	if (!IsValid(m_Start) || !IsValid(m_Player))
		return;
	const auto x = m_Start->GetTransform().TransformPosition(FVector(0, 190, 60));
	m_Player->SetActorLocation(x);
}

//=================================================================================
void AMiniGolfLevel::UpdateDistance()
{
	if (m_LevelGUI)
	{
		auto* distanceGUI = Cast<UTextBlock>(m_LevelGUI->GetWidgetFromName("Distance"));
		if (distanceGUI)
		{
			const auto direction = m_Player->GetActorLocation() - m_Hole->GetActorLocation();
			distanceGUI->SetText(FText::Format(LOCTEXT("Distance", "Distance {0}m"), direction.Size()/1000.f));
		}
	}
}

//=================================================================================
void AMiniGolfLevel::CoinCollected()
{
	NumCollectedCoins++;
	UpdateCoinsText();
}

#undef LOCTEXT_NAMESPACE