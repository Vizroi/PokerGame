// Fill out your copyright notice in the Description page of Project Settings.


#include "CardGameState.h"
#include "PlayerStateCustom.h"
#include "CardPlayerController.h"
#include <Net/UnrealNetwork.h>


ACardGameState::ACardGameState()
{
	bReplicates = true;

	RevealedPlayers.Empty();
	GameScore = 0;
	CurrentGamePhase = EGamePhase::WaitingForPlayers;
	CurrentPlayerIndex = -1;
	LastPlayerIndex = -1;
	LastPlayCardsPlayerIndex = -1;
	PlayerLastCards.Empty();
	CurrentAskWindPlayerIndex = -1;
	WindActionInfoArray.Empty();
	IsGameEnd = false;
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

void ACardGameState::ResetCardGameStateData()
{
	for (int32 i = 0; i < PlayerStateArray.Num(); i++)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			PlayerState->ResetPlayerStateData();
		}
	}


	RevealedPlayers.Empty();
	GameScore = 0;
	CurrentGamePhase = EGamePhase::WaitingForPlayers;
	CurrentPlayerIndex = -1;
	LastPlayerIndex = -1;
	LastPlayCardsPlayerIndex = -1;
	PlayerLastCards.Empty();
	CurrentAskWindPlayerIndex = -1;
	WindActionInfoArray.Empty();
	IsGameEnd = false;
}

void ACardGameState::ResetWindActionData()
{
	WindActionInfoArray.Empty();
	CurrentAskWindPlayerIndex = -1;
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

void ACardGameState::NotifyLastPlayCardSetChange()
{
	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			ACardPlayerController* PC = Cast<ACardPlayerController>(PlayerState->GetPlayerController());
			if(PC)
			{
				PC->ClientUpdateLastPlayCards(PlayerLastCards, CurrentPlayerIndex);
			}
		}
	}
}

void ACardGameState::NotifyCurrentPlayerIndexChange()
{
	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			ACardPlayerController* PC = Cast<ACardPlayerController>(PlayerState->GetPlayerController());
			if (PC)
			{
				PC->ClientUpdateCurPlayerIdxChange(CurrentPlayerIndex, LastPlayCardsPlayerIndex);
			}
		}
	}
}

void ACardGameState::NotifyCurAskWindPlayerIndexChange(int32 CurAskWindPlayerIdx)
{
	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			ACardPlayerController* PC = Cast<ACardPlayerController>(PlayerState->GetPlayerController());
			if (PC)
			{
				PC->ClientUpdateCurAskWindPlayerIndex(CurrentAskWindPlayerIndex);
			}
		}
	}
}

void ACardGameState::NotifyWindResult(int32 PlayerIdx, EWindResultType Type)
{
	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			ACardPlayerController* PC = Cast<ACardPlayerController>(PlayerState->GetPlayerController());
			if (PC)
			{
				//NotifyCurAskWindPlayerIndexChange(CurrentAskWindPlayerIndex);
				PC->ClientWindActionResult(PlayerIdx, Type);
			}
		}
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

void ACardGameState::SetGameEnd(bool IsEnd)
{
	if(!HasAuthority())
	{
		return;
	}

	if (IsGameEnd != IsEnd)
	{
		IsGameEnd = IsEnd;
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

	//GameScore = DefaultGameScore;
	GameScore = 10;
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

int32 ACardGameState::GetNextCanPlayCardsPlayer(int32 CurPlayerIdx)
{
	int32 NextPlayerIdx = CurPlayerIdx;
	if (NextPlayerIdx < 0)
	{
		for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
		{
			APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
			if (PlayerState)
			{
				bool HasHeartSix = PlayerState->HasCard(ESuit::Heart, ECardValue::Six);
				if (HasHeartSix)
				{
					NextPlayerIdx = PlayerState->GetPlayerIndex();
				}
			}
		}
	}
	else
	{
		int32 ProtectCount = 99;

		int32 EndInex = NextPlayerIdx + 1;
		do
		{
			if(ProtectCount-- <= 0)
			{
				UE_LOG(LogTemp, Error, TEXT("Server :  GetNextCanPlayCardsPlayer is bad!!! "));
				break;
			}


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

		} while (EndInex != NextPlayerIdx);

		NextPlayerIdx = EndInex;
	}

	return NextPlayerIdx;
}

void ACardGameState::MoveToNextPlayer()
{
	if (!HasAuthority())
	{
		return;
	}

	if (GetGameEnd())
	{
		return;
	}

	LastPlayerIndex = CurrentPlayerIndex;

	CurrentPlayerIndex = GetNextCanPlayCardsPlayer(CurrentPlayerIndex);

	NotifyCurrentPlayerIndexChange();

	UE_LOG(LogTemp, Warning, TEXT("Server :               CurrentPlayerIndex: %d"), CurrentPlayerIndex);
	UE_LOG(LogTemp, Warning, TEXT("Server :               LastPlayerIndex: %d"), LastPlayerIndex);
}

void ACardGameState::MoveToPlayer(int32 PlayerIdx)
{
	if (!HasAuthority())
	{
		return;
	}

	if (GetGameEnd())
	{
		return;
	}

	LastPlayerIndex = CurrentPlayerIndex;
	CurrentPlayerIndex = PlayerIdx;

	NotifyCurrentPlayerIndexChange();

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

int32 ACardGameState::GetLeftPlayerCount()
{
	int32 LeftPlayerCounts = PlayerStateArray.Num();
	LeftPlayerCounts -= GetFinishPlayerCount();
	return LeftPlayerCounts;
}

int32 ACardGameState::GetFinishPlayerCount()
{
	int32 FinishPlayerCount = 0;
	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			if (PlayerState->GetFinishHandCards())
			{
				FinishPlayerCount++;
			}
		}
	}
	return FinishPlayerCount;
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

void ACardGameState::EmptyLastCardSetByLastPlayCards()
{
	if (!HasAuthority())
	{
		return;
	}

	for(auto& Elem : PlayerLastCards)
	{
		if (Elem.PlayerIndex == LastPlayCardsPlayerIndex)
		{
			Elem.LastCards.Empty();
			break;
		}
	}
}

int32 ACardGameState::GetNextAskWindPlayerIdx(int32 CurPlayerIdx)
{
	int32 NextAskWindPlayerIdx = CurPlayerIdx;
	if (NextAskWindPlayerIdx < 0)
	{
		NextAskWindPlayerIdx = CurrentPlayerIndex;
	}
	else
	{
		NextAskWindPlayerIdx = GetNextCanPlayCardsPlayer(NextAskWindPlayerIdx);
	}

	return NextAskWindPlayerIdx;
}


void ACardGameState::AskNextPlayerGrabWind()
{
	if (!HasAuthority())
	{
		return;
	}

	CurrentAskWindPlayerIndex = GetNextAskWindPlayerIdx(CurrentAskWindPlayerIndex);

	NotifyCurAskWindPlayerIndexChange(CurrentAskWindPlayerIndex);
}

void ACardGameState::UpdateWindActionInfo(FWindActionInfo WindActionInfo)
{
	if (!HasAuthority())
	{
		return;
	}

	if (WindActionInfoArray.IsEmpty())
	{
		HandleFirstPlayerWindAction(WindActionInfo);
	}
	else
	{
		HandleSubsequentPlayerWindAction(WindActionInfo);
	}
}

void ACardGameState::AddWindActionInfo(FWindActionInfo WindActionInfo)
{
	if (!HasAuthority())
	{
		return;
	}

	for (int32 i = 0; i < WindActionInfoArray.Num(); ++i)
	{
		if (WindActionInfoArray[i].PlayerIdx == WindActionInfo.PlayerIdx)
		{
			WindActionInfoArray[i] = WindActionInfo;
			return;
		}
	}
	WindActionInfoArray.Add(WindActionInfo);
}

void ACardGameState::HandleFirstPlayerWindAction(FWindActionInfo WindActionInfo)
{
	AddWindActionInfo(WindActionInfo);
	if (WindActionInfo.WindType == EWindType::EWT_GrabWind)
	{
		APlayerStateCustom* PS = GetPlayerStateByIndex(WindActionInfo.PlayerIdx);
		if (PS)
		{
			ACardPlayerController* APC = Cast<ACardPlayerController>(PS->GetPlayerController());
			if (APC)
			{
				ResetWindActionData();
				NotifyWindResult(WindActionInfo.PlayerIdx, EWindResultType::EWRT_GrabWindSuccess);
			}
		}
	}
	else
	{
		AskNextPlayerGrabWind();
	}
}

void ACardGameState::HandleSubsequentPlayerWindAction(FWindActionInfo WindActionInfo)
{
	APlayerStateCustom* FirstPS = GetPlayerStateByIndex(WindActionInfoArray[0].PlayerIdx);
	if (FirstPS)
	{
		AddWindActionInfo(WindActionInfo);

		if (WindActionInfo.WindType == EWindType::EWT_WardWind)
		{
			if (WindActionInfoArray.Num() == GetLeftPlayerCount())
			{
				EmptyLastCardSetByLastPlayCards();
				ResetWindActionData();
				NotifyWindResult(WindActionInfoArray[0].PlayerIdx, EWindResultType::EWRT_WardWindSuccess);
			}
			else
			{
				AskNextPlayerGrabWind();
			}
		}
		else if (WindActionInfo.WindType == EWindType::EWT_GrabWind)
		{
			ACardPlayerController* APC = Cast<ACardPlayerController>(FirstPS->GetPlayerController());
			if (APC)
			{
				if (WindActionInfo.PlayerIdx == WindActionInfoArray[0].PlayerIdx)
				{
					ResetWindActionData();
					NotifyWindResult(WindActionInfo.PlayerIdx, EWindResultType::EWRT_GrabWindSuccess);
				}
				else
				{
					NotifyWindResult(WindActionInfoArray[0].PlayerIdx, EWindResultType::EWRT_WardWindFail);
				}
			}
		}
		else if (WindActionInfo.WindType == EWindType::EWT_GiveUpWind)
		{
			for(auto Elem : WindActionInfoArray)
			{
				if (Elem.WindType == EWindType::EWT_GrabWind)
				{
					ResetWindActionData();
					NotifyWindResult(Elem.PlayerIdx, EWindResultType::EWRT_GrabWindSuccess);
					MoveToPlayer(Elem.PlayerIdx);
					break;
				}
			}
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

//void ACardGameState::OnRep_CurrentPlayerIndexChange()
//{
//	ACardPlayerController* PC = Cast<ACardPlayerController>(GetWorld()->GetFirstPlayerController());
//	if (PC)
//	{
//		PC->OnCurrentPlayerIndexChange(CurrentPlayerIndex, LastPlayCardsPlayerIndex);
//	}
//}

//void ACardGameState::OnRep_PlayerLastCardsChange()
//{
//	ACardPlayerController* PC = Cast<ACardPlayerController>(GetWorld()->GetFirstPlayerController());
//	if (PC)
//	{
//		PC->OnPlayerLastCardsChange(PlayerLastCards);
//	}
//}

//void ACardGameState::OnRep_CuirAskWindPlayerIndexChange()
//{
//	ACardPlayerController* PC = Cast<ACardPlayerController>(GetWorld()->GetFirstPlayerController());
//	if (PC)
//	{
//		PC->OnCurAskWindPlayerIndexChange(CurrentAskWindPlayerIndex);
//	}
//}
