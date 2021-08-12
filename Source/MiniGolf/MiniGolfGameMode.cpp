// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniGolfGameMode.h"
#include "MiniGolfBall.h"

AMiniGolfGameMode::AMiniGolfGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = AMiniGolfBall::StaticClass();
}
