// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGolfLevel.h"
#include "Kismet/GameplayStatics.h"

#include "Coin.h"

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
}

//=================================================================================
void AMiniGolfLevel::OnBallHit()
{
	NumHits++;
}
