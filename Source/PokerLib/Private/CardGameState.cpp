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
	LastPlayerIndex = -1;
	LastPlayCardsPlayerIndex = -1;
	PlayerLastCards.Empty();
}

void ACardGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACardGameState, PlayerStateArray);
	DOREPLIFETIME(ACardGameState, GameScore);
	DOREPLIFETIME(ACardGameState, RevealedPlayers);
	DOREPLIFETIME(ACardGameState, CurrentGamePhase);
	DOREPLIFETIME(ACardGameState, CurrentPlayerIndex);
	DOREPLIFETIME(ACardGameState, LastPlayerIndex);
	DOREPLIFETIME(ACardGameState, PlayerLastCards);
	DOREPLIFETIME(ACardGameState, LastPlayCardsPlayerIndex);
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

	LastPlayerIndex = CurrentPlayerIndex;

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
		int32 EndInex = CurrentPlayerIndex + 1;
		do
		{
			if (EndInex >= PlayerStateArray.Num())
			{
				EndInex = 0;
			}

			APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[EndInex]);
			if (PlayerState)
			{
				if (PlayerState->GetFinishHandCards())
				{
					EndInex++;
				}
				else
				{
					break;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Server :               PlayerState is nullptr"));
				break;
			}

		} while (EndInex != CurrentPlayerIndex);

		CurrentPlayerIndex = EndInex;
	}


	UE_LOG(LogTemp, Warning, TEXT("Server :               CurrentPlayerIndex: %d"), CurrentPlayerIndex);
	UE_LOG(LogTemp, Warning, TEXT("Server :               LastPlayerIndex: %d"), LastPlayerIndex);
}

void ACardGameState::SetPlayerIndexGameOverType(int32 InPlayerIndex, EGameOverType Type)
{
	if (!HasAuthority())
	{
		return;
	}

	APlayerStateCustom* PlayerState = GetPlayerStateByIndex(InPlayerIndex);
	if (PlayerState)
	{
		PlayerState->SetGameOverType(Type);
	}
}

EGameOverType ACardGameState::GetPlayerIndexGameOverType(int32 InPlayerIndex)
{
	EGameOverType Type = EGameOverType::None;
	APlayerStateCustom* PlayerState = GetPlayerStateByIndex(InPlayerIndex);
	if (PlayerState)
	{
		Type = PlayerState->GetGameOverType();
	}
	return Type;
}

ETeamID ACardGameState::GetPlayerIndexTeamID(int32 InPlayerIndex)
{
	ETeamID ID = ETeamID::InValid;
	APlayerStateCustom* PlayerState = GetPlayerStateByIndex(InPlayerIndex);
	if (PlayerState)
	{
		ID = PlayerState->GetTeamID();
	}
	return ID;
}

void ACardGameState::AddLastCardSet(int32 PlayerIndex, TArray<FCard> LastCards)
{
	if(!HasAuthority())
	{
		return;
	}

	bool bFound = false;
	for (auto& Elem : PlayerLastCards)
	{
		if(Elem.PlayerIndex == PlayerIndex)
		{
			bFound = true;
			Elem.LastCards.Empty();
			Elem.LastCards = LastCards;
			break;
		}
	}

	if (!bFound)
	{
		FLastCardSet LastCardSet;
		LastCardSet.PlayerIndex = PlayerIndex;
		LastCardSet.LastCards = LastCards;
		PlayerLastCards.Add(LastCardSet);
	}

	if (LastCards.Num() > 0)
	{
		LastPlayCardsPlayerIndex = PlayerIndex;
	}
}

TArray<FCard> ACardGameState::GetLastCardSetByPlayerIndex(int32 PlayerIndex)
{
	TArray<FCard> CardsArr;

	if (GetCurrentPlayerIndex() != GetLastPlayCardsPlayerIndex() && GetLastPlayCardsPlayerIndex() != -1)
	{
		for (auto Elem : PlayerLastCards)
		{
			if (Elem.PlayerIndex == PlayerIndex)
			{
				CardsArr = Elem.LastCards;
			}
		}
	}

	return CardsArr;
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
		PC->OnCurrentPlayerIndexChange(CurrentPlayerIndex, LastPlayCardsPlayerIndex);
	}
}

void ACardGameState::OnRep_PlayerLastCardsChange()
{
	ACardPlayerController* PC = Cast<ACardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->OnPlayerLastCardsChange(PlayerLastCards);
	}
}
