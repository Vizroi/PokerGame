// Copyright Epic Games, Inc. All Rights Reserved.

#include "PokerLibPlayerController.h"

APokerLibPlayerController::APokerLibPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
