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
			CardGameState->ChangeGamePhase(EGamePhase::WaitingForPlayers);
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
		//ReStartGame();
		bGameStarted = true;
		FinshiPlayers.Empty();
		InitializeCards(EDeckMode::RedTen);
		DealCardsToPlayers();
		AssignTeams();
		ReSetGameScore();
	}
}

void ACardGameMode::ReStartGame()
{
	ACardGameState* CardGameState = GetGameState<ACardGameState>();
	if (CardGameState)
	{
		bGameStarted = false;
		CardGameState->ResetCardGameStateData();
		CardGameState->ChangeGamePhase(EGamePhase::WaitingForPlayers);
	}
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
		CardGameState->ChangeGamePhase(EGamePhase::DealingCards);
		CardGameState->DealCardToPlayer(GameDeck);
	}
}

void ACardGameMode::AssignTeams()
{
	ACardGameState* CardGameState = GetGameState<ACardGameState>();
	if (CardGameState)
	{
		CardGameState->ChangeGamePhase(EGamePhase::AssigningTeams);
		CardGameState->AssignTeam();
		CardGameState->ChangeGamePhase(EGamePhase::RevealingTeams);
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

void ACardGameMode::OnPlayerFinishPlayCards(int32 PlayerIndex)
{
	if (FinshiPlayers.Contains(PlayerIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player %d has already finished playing cards"), PlayerIndex)
		return;
	}


	FinshiPlayers.Add(PlayerIndex);
	if (CheckGameOver())
	{
		EndGame();
	}
}

bool ACardGameMode::CheckGameOver()
{
	ACardGameState* CardGameState = GetGameState<ACardGameState>();
	if (!CardGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("No CardGameState found"));
		return false;
	}

	bool IsGameOver = false;

	if (FinshiPlayers.Num() == 1)
	{
		ETeamID TeamID_index_0 = CardGameState->GetPlayerIndexTeamID(FinshiPlayers[0]);
		
		// first is two pair red ten game is over
		if (TeamID_index_0 == ETeamID::TwoPairRedTen)
		{
			IsGameOver = true;
		}
		else
		{
			IsGameOver = false;
		}
	}
	else if (FinshiPlayers.Num() == 2)
	{
		ETeamID TeamID_index_0 = CardGameState->GetPlayerIndexTeamID(FinshiPlayers[0]);
		ETeamID TeamID_index_1 = CardGameState->GetPlayerIndexTeamID(FinshiPlayers[1]);

		if (TeamID_index_1 == ETeamID::TwoPairRedTen)
		{

			IsGameOver = true;
		}
		else if (TeamID_index_0 == ETeamID::RedTen_Diamond && TeamID_index_1 == ETeamID::RedTen_Heart)
		{
			IsGameOver = true;
		}
		else if (TeamID_index_1 == ETeamID::RedTen_Diamond && TeamID_index_0 == ETeamID::RedTen_Heart)
		{

			IsGameOver = true;
		}
		else
		{
			IsGameOver = false;
		}
	}
	else if (FinshiPlayers.Num() == 3)
	{
		IsGameOver = true;
	}

	return IsGameOver;
}

void ACardGameMode::EndGame()
{
	ACardGameState* CardGameState = GetGameState<ACardGameState>();
	if (!CardGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("No CardGameState found"));
		return;
	}

	TArray<APlayerStateCustom*> PlayerStatesArray = CardGameState->GetPlayerStateArr();
	for (int32 i = 0; i < PlayerStatesArray.Num(); i++)
	{
		APlayerStateCustom* PS = PlayerStatesArray[i];
		if (PS)
		{
			int32 PSIndex = PS->GetPlayerIndex();
			if (PSIndex != FinshiPlayers[0] && !FinshiPlayers.Contains(PSIndex))
			{
				PS->SetFinishHandCards(true);
				FinshiPlayers.Add(PSIndex);
			}
		}
	}

	ETeamID TeamID_index_0 = CardGameState->GetPlayerIndexTeamID(FinshiPlayers[0]);
	ETeamID TeamID_index_1 = CardGameState->GetPlayerIndexTeamID(FinshiPlayers[1]);
	ETeamID TeamID_index_2 = CardGameState->GetPlayerIndexTeamID(FinshiPlayers[2]);
	ETeamID TeamID_index_3 = CardGameState->GetPlayerIndexTeamID(FinshiPlayers[3]);
	if (TeamID_index_0 == ETeamID::TwoPairRedTen)
	{
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[0], EGameOverType::Win);
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[1], EGameOverType::Lose);
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[2], EGameOverType::Lose);
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[3], EGameOverType::Lose);
	}
	else if (TeamID_index_1 == ETeamID::TwoPairRedTen || TeamID_index_2 == ETeamID::TwoPairRedTen)
	{
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[0], EGameOverType::Draw);
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[1], EGameOverType::Draw);
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[2], EGameOverType::Draw);
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[3], EGameOverType::Draw);
	}
	else if (TeamID_index_3 == ETeamID::TwoPairRedTen)
	{
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[0], EGameOverType::Lose);
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[1], EGameOverType::Win);
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[2], EGameOverType::Win);
		CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[3], EGameOverType::Win);
	}
	else if (TeamID_index_0 == ETeamID::RedTen_Heart || TeamID_index_0 == ETeamID::RedTen_Diamond)
	{
		if (TeamID_index_1 == ETeamID::RedTen_Heart || TeamID_index_1 == ETeamID::RedTen_Diamond || TeamID_index_2 == ETeamID::RedTen_Heart ||TeamID_index_2 == ETeamID::RedTen_Diamond)
		{
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[0], EGameOverType::Win);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[1], EGameOverType::Win);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[2], EGameOverType::Lose);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[3], EGameOverType::Lose);
		}
		else
		{
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[0], EGameOverType::Draw);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[1], EGameOverType::Draw);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[2], EGameOverType::Draw);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[3], EGameOverType::Draw);
		}
	}
	else if (TeamID_index_0 == ETeamID::NoRedTen)
	{
		if (TeamID_index_1 == ETeamID::NoRedTen || TeamID_index_2 == ETeamID::NoRedTen)
		{
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[0], EGameOverType::Win);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[1], EGameOverType::Win);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[2], EGameOverType::Lose);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[3], EGameOverType::Lose);
		}
		else
		{
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[0], EGameOverType::Draw);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[1], EGameOverType::Draw);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[2], EGameOverType::Draw);
			CardGameState->SetPlayerIndexGameOverType(FinshiPlayers[3], EGameOverType::Draw);
		}
	}

	CardGameState->ChangeGamePhase(EGamePhase::GameOver);
}
