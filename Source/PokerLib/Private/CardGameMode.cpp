// Fill out your copyright notice in the Description page of Project Settings.


#include "CardGameMode.h"
#include "PlayerStateCustom.h"
#include "CardGameState.h"

ACardGameMode::ACardGameMode()
{
	DefaultPawnClass = NULL;
	PlayerStateClass = APlayerStateCustom::StaticClass();
	PlayerControllerClass = ACardPlayerController::StaticClass();
	GameStateClass = ACardGameState::StaticClass();
}

void ACardGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APlayerStateCustom* NewPlayerState = Cast<APlayerStateCustom>(NewPlayer->PlayerState);
	if (NewPlayerState)
	{
		// set default player name
		NewPlayerState->SetPlayerName(FString::Printf(TEXT("Poker_Player_%d"), NewPlayer->PlayerState->PlayerId));

		int32 PlayerIndex = -1;

		ACardGameState* CardGameState = GetGameState<ACardGameState>();
		if (CardGameState)
		{
			for (int32 i = 0; i < 4; i++)
			{
				uint8 Mask = 1 << i;
				if (CardGameState->AvailableIndices & Mask)
				{
					PlayerIndex = i;
					CardGameState->AvailableIndices &= ~Mask;
					break;
				}
			}

			NewPlayerState->SetPlayerIndex(PlayerIndex);
			NewPlayerState->SetCustomPlayerName(FString::Printf(TEXT("Player_%d"), PlayerIndex));
			NewPlayerState->SetPlayerReady(false);

			CardGameState->AddPlayerStateArray(NewPlayerState);
		}
	}

	//CardPlayerController
	ACardPlayerController* CardPlayerController = Cast<ACardPlayerController>(NewPlayer);
	if (CardPlayerController)
	{
		CardPlayerController->ClientCreateGameMenuUI();
	}
}

void ACardGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	APlayerStateCustom* ExitPlayerState = Exiting->GetPlayerState<APlayerStateCustom>();
	if (ExitPlayerState)
	{
		ACardGameState* CardGameState = GetGameState<ACardGameState>();
		if (CardGameState)
		{
			uint8 Mask = 1 << ExitPlayerState->GetPlayerIndex();
			CardGameState->AvailableIndices |= Mask;

			CardGameState->RemovePlayerStateArray(ExitPlayerState);
		}
	}
}

void ACardGameMode::OnPlayerReady()
{
	int32 PlayerReadyCount = 0;
	ACardGameState* CardGameState = GetGameState<ACardGameState>();
	if (CardGameState)
	{
		PlayerReadyCount = CardGameState->GetPlayerStateReadyCount();
	}

	if (PlayerReadyCount == 4)
	{
		StartGame();
	}
}

void ACardGameMode::StartGame()
{
	if (!bGameStarted)
	{
		bGameStarted = true;
		ReStartGame();
	}
}

void ACardGameMode::ReStartGame()
{
	InitializeCards(EDeckMode::RedTen);
	DealCardsToPlayers();
	AssignTeams();
	ReSetGameScore();
}

void ACardGameMode::InitializeCards(EDeckMode Mode)
{
	if (GameDeck == nullptr) 
	{
		GameDeck = NewObject<UDeck>(this,UDeck::StaticClass());
		GameDeck->InitializeDeck(Mode);
	}
	else 
	{
		GameDeck->InitializeDeck(Mode);
	}
}

void ACardGameMode::DealCardsToPlayers()
{
	ACardGameState* CardGameState = GetGameState<ACardGameState>();
	if (CardGameState)
	{
		CardGameState->DealCardToPlayer(GameDeck);
	}
}

void ACardGameMode::AssignTeams()
{
	ACardGameState* CardGameState = GetGameState<ACardGameState>();
	if (CardGameState)
	{
		CardGameState->AssignTeam();
	}
}

void ACardGameMode::ReSetGameScore()
{
	ACardGameState* CardGameState = GetGameState<ACardGameState>();
	if (CardGameState)
	{
		CardGameState->ResetGameScore();
	}
}
