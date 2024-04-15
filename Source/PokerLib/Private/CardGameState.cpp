// Fill out your copyright notice in the Description page of Project Settings.


#include "CardGameState.h"
#include <Net/UnrealNetwork.h>
#include "CardPlayerController.h"

ACardGameState::ACardGameState()
{
	bReplicates = true;
	GameScore = 0;
	CurrentGamePhase = EGamePhase::WaitingForPlayers;
	CurrentPlayerIndex = -1;
}

void ACardGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACardGameState, PlayerStateArray);
	DOREPLIFETIME(ACardGameState, GameScore);
	DOREPLIFETIME(ACardGameState, RevealedPlayers);
	DOREPLIFETIME(ACardGameState, CurrentGamePhase);
	DOREPLIFETIME(ACardGameState, CurrentPlayerIndex);
}

void ACardGameState::ChangeGamePhase(EGamePhase NewGamePhase)
{
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentGamePhase != NewGamePhase)
	{
		CurrentGamePhase = NewGamePhase;
	}
}

APlayerStateCustom* ACardGameState::GetPlayerStateByIndex(int32 index)
{
	//±éÀúÕû¸öPlayerStateArray
	for (int32 i = 0; i < PlayerStateArray.Num(); i++)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			if(PlayerState->GetPlayerIndex() == index)
			{
				return PlayerState;
			}
		}
	}
	return nullptr;
}

void ACardGameState::AddPlayerStateArray(APlayerStateCustom* NewPlayerState)
{
	PlayerStateArray.Add(NewPlayerState);
}

void ACardGameState::RemovePlayerStateArray(APlayerStateCustom* PlayerState)
{
	PlayerStateArray.Remove(PlayerState);
}

int32 ACardGameState::GetPlayerStateReadyCount()
{
	int32 ReadyCount = 0;
	for (int32 i = 0; i < PlayerStateArray.Num(); i++)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			if (PlayerState->GetPlayerReady())
			{
				ReadyCount++;
			}
		}
	}
	return ReadyCount;
}

void ACardGameState::DealCardToPlayer(UDeck* CardDeck)
{
	if (!HasAuthority())
	{
		return;
	}

	for (int32 i = 0; i < PlayerStateArray.Num(); i++)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			PlayerState->ClearHandCards();

			for (int32 j = 0; j < 10; ++j)
			{
				FCard Card = CardDeck->DealOneCard();
				PlayerState->AddCardToHand(Card);
				PlayerState->SortHandCards();
			}
		}
	}
}

void ACardGameState::AssignTeam()
{
	if (!HasAuthority())
	{
		return;
	}

	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PS)
		{
			PS->AssignTeamID();
		}
	}
}

void ACardGameState::RevealAllIdentiy()
{
	if (!HasAuthority())
	{
		return;
	}

	RevealedPlayers.Empty();

	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PS)
		{
			PS->SetIdentityStatus(EIdentityStatus::Identity);
			FPlayerTeamInfo TeamInfo;
			TeamInfo.PlayerState = PS;
			TeamInfo.TeamID = PS->GetTeamID();
			RevealedPlayers.Add(TeamInfo);
		}
	}
}

bool ACardGameState::CheckAllPlayerIdentityStatusValid()
{
	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PS)
		{
			EIdentityStatus Status = PS->GetIdentityStatus();
			if (Status == EIdentityStatus::InValid)
			{
				return false;
			}
		}
	}
	return true;
}

void ACardGameState::MultiplyGameScore()
{
	if (!HasAuthority())
	{
		return;
	}

	GameScore *= 2;
	//GameScore  += DefaultGameScore;
}

void ACardGameState::ResetGameScore()
{
	if (!HasAuthority())
	{
		return;
	}

	GameScore = DefaultGameScore;
}

void ACardGameState::AddGameScore(int32 Score)
{
	if (!HasAuthority())
	{
		return;
	}

	GameScore += Score;
}

int32 ACardGameState::GetGameScore()
{
	return GameScore;
}

void ACardGameState::SetCurrentPlayerIndex(int32 Index)
{
	if (!HasAuthority())
	{
		CurrentPlayerIndex = Index;
	}
}

void ACardGameState::MoveToNextPlayer()
{
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentPlayerIndex < 0)
	{
		for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
		{
			APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
			if (PlayerState)
			{
				bool HasHeartSix = PlayerState->HasCard(ESuit::Heart, ECardValue::Six);
				if (HasHeartSix)
				{
					CurrentPlayerIndex = PlayerState->GetPlayerIndex();
				}
			}
		}
	}
	else
	{
		CurrentPlayerIndex++;
		if (CurrentPlayerIndex >= PlayerStateArray.Num())
		{
			CurrentPlayerIndex = 0;
		}
	}
}

void ACardGameState::OnRep_GamePhaseChange()
{
	ACardPlayerController* PC = Cast<ACardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->OnGamePhaseChange(CurrentGamePhase);
	}
}

void ACardGameState::OnRep_PlayerStateArrayChange()
{
	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			PlayerState->NotifyPlayerJoinMainMenuBase();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CardGameState: %s, PlayerStateArray.Num(): %d"), *GetName(), PlayerStateArray.Num());
}

void ACardGameState::OnRep_RevealedTeamInfo()
{
	ACardPlayerController* PC = Cast<ACardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->OnRevealAllIdentity(RevealedPlayers);
	}
}

void ACardGameState::OnRep_GameScoreChange()
{
	ACardPlayerController* PC = Cast<ACardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->OnUpdateGameScore(GameScore);
	}
}

void ACardGameState::OnRep_CurrentPlayerIndexChange()
{
	ACardPlayerController* PC = Cast<ACardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->OnCurrentPlayerIndexChange(CurrentPlayerIndex);
	}
}
