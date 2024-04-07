// Copyright Epic Games, Inc. All Rights Reserved.

#include "PokerLibGameMode.h"
#include "PokerLibPlayerController.h"
#include "PokerLibPawn.h"

APokerLibGameMode::APokerLibGameMode()
{
	// no pawn by default
	DefaultPawnClass = APokerLibPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = APokerLibPlayerController::StaticClass();
}
