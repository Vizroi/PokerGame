// Fill out your copyright notice in the Description page of Project Settings.


#include "CardGameState.h"
#include <Net/UnrealNetwork.h>
#include "CardPlayerController.h"

ACardGameState::ACardGameState()
{
	bReplicates = true;
	GameScore = 0;
	CurrentGamePhase = EGamePhase::WaitingForPlayers;
}

void ACardGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACardGameState, PlayerStateArray);
	DOREPLIFETIME(ACardGameState, GameScore);
	DOREPLIFETIME(ACardGameState, RevealedPlayers);
	DOREPLIFETIME(ACardGameState, CurrentGamePhase);
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
	//遍历整个PlayerStateArray
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

void ACardGameState::RevealAllIdentiy(bool IsReveal)
{
	if (!HasAuthority())
	{
		return;
	}

	//TODO ;;;; IdentityStatus 根据该值来判断是否所有人都决策完毕，然后通知给所有人，然后开始发牌

	if (IsReveal)
	{
		RevealedPlayers.Empty();

		for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
		{
			APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerStateArray[i]);
			if (PS)
			{
				FPlayerTeamInfo TeamInfo;
				TeamInfo.PlayerState = PS;
				TeamInfo.TeamID = PS->GetTeamID();
				RevealedPlayers.Add(TeamInfo);
			}
		}
	}
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
