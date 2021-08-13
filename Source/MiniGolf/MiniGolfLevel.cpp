// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGolfLevel.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MiniGolf/MiniGolfBall.h"

#include "Coin.h"

#define LOCTEXT_NAMESPACE "MyNamespace"


//=================================================================================
void AMiniGolfLevel::BeginPlay()
{	
	Super::BeginPlay();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoin::StaticClass(), FoundActors);
	for (auto* actor : FoundActors)
	{
		auto* coin = Cast<ACoin>(actor);
		//coin->CoinCollected.a;
	}
	NumCoins = FoundActors.Num();

	FStringClassReference LevelGUIClassRef(TEXT("/Game/GUI/LevelGUI"));
	const auto* LevelGUIClass = LevelGUIClassRef.TryLoadClass<UUserWidget>();

	m_LevelGUI = CreateWidget<UUserWidget>(GetGameInstance(), LevelGUI);
	UpdateHitsGUI();
	UpdateLevelName();
	UpdateCoinsText();
	if(m_LevelGUI)
		m_LevelGUI->AddToViewport(9999);
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
			numHits->SetText(FText::Format(LOCTEXT("CoinsCollected", "Coins {0}/{1}"), 0, NumCoins));
		}
	}
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
			distanceGUI->SetText(FText::Format(LOCTEXT("Distance", "Distance {0}m"), direction.Size()));
		}
	}
}

#undef LOCTEXT_NAMESPACE