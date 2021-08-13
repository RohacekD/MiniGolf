// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MiniGolfGameMode.generated.h"

UCLASS(minimalapi)
class AMiniGolfGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMiniGolfGameMode();
protected:
	//=================================================================================
	void BeginPlay() override;

};



