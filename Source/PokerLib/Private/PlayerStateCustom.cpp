// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateCustom.h"
#include "CardGameMode.h"
#include "Net/UnrealNetwork.h"
#include "CardPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "RedTenCardFunctionLibrary.h"

APlayerStateCustom::APlayerStateCustom()
{
	bReplicates = true;
    PlayerIndex = -1;
    PlayerCustomName = FString(TEXT("None"));

    bIsReady = false;
    HandCards.Empty();
    HandCardsCount = 0;
    PlayerBetScore = 0;
    TeamID = ETeamID::InValid;
    bIsFinishHandCards = false;
    GameOverType = EGameOverType::None;
}

void APlayerStateCustom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APlayerStateCustom, PlayerIndex);
    DOREPLIFETIME(APlayerStateCustom, PlayerCustomName);
    DOREPLIFETIME(APlayerStateCustom, bIsReady); 
    DOREPLIFETIME(APlayerStateCustom, HandCardsCount);
    DOREPLIFETIME(APlayerStateCustom, PlayerBetScore);
    DOREPLIFETIME(APlayerStateCustom, bIsFinishHandCards);
    DOREPLIFETIME(APlayerStateCustom, GameOverType);
    DOREPLIFETIME_CONDITION(APlayerStateCustom, HandCards, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(APlayerStateCustom, TeamID, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(APlayerStateCustom, IdentityStatus, COND_OwnerOnly);
}

void APlayerStateCustom::SetPlayerIndex(int32 Index)
{
    if (GetLocalRole() == ROLE_Authority)
    {
		PlayerIndex = Index;
    }
}

void APlayerStateCustom::SetCustomPlayerName(const FString& NewName)
{
    PlayerCustomName = NewName;
}

void APlayerStateCustom::SetPlayerReady(bool value)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        bIsReady = value;
    }
}

void APlayerStateCustom::ClearHandCards()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		HandCards.Empty();
		UpdateCardsCount();
	}
}

void APlayerStateCustom::AddCardToHand(const FCard& Card)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        HandCards.Add(Card);
        UpdateCardsCount();
    }
}

void APlayerStateCustom::RemoveCardToHand(const FCard& Card)
{
    if (GetLocalRole() == ROLE_Authority)
	{
        for(int32 i = 0; i < HandCards.Num(); i++)
		{
			if (HandCards[i].Suit == Card.Suit && HandCards[i].Value == Card.Value)
			{
				HandCards.RemoveAt(i);
                break;
			}
		}
		UpdateCardsCount();
	}
}

void APlayerStateCustom::RemoveCardToHandFormCardId(const TArray<int32>& CardId)
{
    if (GetLocalRole() == ROLE_Authority)
	{
        //PrintHandsCardsInfo("Before RemoveCardToHandFromCardId: ");

		for(int32 i = 0; i < CardId.Num(); i++)
		{
			for(int32 j = 0; j < HandCards.Num(); j++)
			{
				if (HandCards[j].CardID == CardId[i])
				{
					HandCards.RemoveAt(j);
                    j--;
					break;
				}
			}
		}
		UpdateCardsCount();

        //PrintHandsCardsInfo("After RemoveCardToHandFromCardId: ");
	}
}

void APlayerStateCustom::SortHandCards()
{
    URedTenCardFunctionLibrary::SortCardsForPriority(HandCards);
}

bool APlayerStateCustom::SelectCardToHand(int32 CardId)
{
    bool IsSelect = false;
    if (GetLocalRole() == ROLE_Authority)
	{
		for (FCard& Card : HandCards)
		{
			if (Card.CardID == CardId)
			{
				Card.bSelected = !Card.bSelected;
                IsSelect = Card.bSelected;
			}
		}
	}
    return IsSelect;
}

void APlayerStateCustom::ClearSelectedCards()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        for (FCard& Card : HandCards)
		{
			Card.bSelected = false;
		}
	}
}

TArray<int32> APlayerStateCustom::GetAllCardID()
{
    TArray<int32> CardIDArray;
    for (FCard& Card : HandCards)
	{
		CardIDArray.Add(Card.CardID);
	}
    return CardIDArray;
}

TArray<int32> APlayerStateCustom::GetAllSelectedCardID()
{
    TArray<int32> CardIDArray;
	for (FCard& Card : HandCards)
	{
		if (Card.bSelected)
		{
			CardIDArray.Add(Card.CardID);
		}
	}
	return CardIDArray;
}

FCard APlayerStateCustom::GetCardByID(int32 CardID)
{
    FCard EmptyCard;
    for(FCard& Card : HandCards)
	{
		if (Card.CardID == CardID)
		{
            return Card;
		}
	}
    return EmptyCard;
}

void APlayerStateCustom::UpdateCardsCount()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        HandCardsCount = HandCards.Num();

        if (HandCardsCount == 0)
        {
            SetFinishHandCards(true);


            ACardGameMode* GameMode = Cast<ACardGameMode>(GetWorld()->GetAuthGameMode());
            if (GameMode)
            {
                GameMode->OnPlayerFinishPlayCards(PlayerIndex);
            }
        }
    }
}

bool APlayerStateCustom::HasCard(ESuit Suit, ECardValue Value)
{
    for (FCard& Card : HandCards)
    {
        if (Card.Suit == Suit && Card.Value == Value)
        {
			return true;
		}
	}

    return false;
}

void APlayerStateCustom::HasReadTen(bool& HasHeartTen, bool& HasDiamondTen)
{
    HasHeartTen = false;
    HasDiamondTen = false;
    for (FCard& Card : HandCards)
    {
        if (Card.Value == ECardValue::Ten)
        {
            if (Card.Suit == ESuit::Heart)
            {
                HasHeartTen = true;
            }
            else if (Card.Suit == ESuit::Diamond)
            {
                HasDiamondTen = true;
            }
        }
    }
}

void APlayerStateCustom::SetFinishHandCards(bool InValue)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        if (bIsFinishHandCards != InValue)
        {
            bIsFinishHandCards = InValue;
        }
	}
}

int32 APlayerStateCustom::GetSeatIndexByPlayerIndex(int32 Index)
{
    int32 SeatIndex = -1;
    APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(GetWorld()->GetFirstPlayerController()->PlayerState);
    if (PlayerState)
    {
        int32 CurrentPlayerIndex = PlayerState->PlayerIndex;
        SeatIndex = (CurrentPlayerIndex - Index + 4) % 4;
    }
    if (SeatIndex < 0)
    {
        UE_LOG(LogTemp, Error, TEXT("UIGameMainBase::GetSeatIndexByPlayerIndex SeatIndex < 0"))
    }

    return SeatIndex;
}

void APlayerStateCustom::SetGameOverType(EGameOverType Type)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        if (GameOverType != Type)
        {
			GameOverType = Type;
		}
	}
}

void APlayerStateCustom::AssignTeamID()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        bool isHaveHeartTen = false;
        bool isHaveDiamondTen = false;
        HasReadTen(isHaveHeartTen, isHaveDiamondTen);
        if (isHaveHeartTen && isHaveDiamondTen)
        {
            TeamID = ETeamID::TwoPairRedTen;
        }
        else if (isHaveHeartTen || isHaveDiamondTen)
        {
            TeamID = ETeamID::RedTen;
        }
        else
        {
            TeamID = ETeamID::NoRedTen;
        }
    }
}

void APlayerStateCustom::SetIdentityStatus(EIdentityStatus Status)
{
    if (GetLocalRole() == ROLE_Authority)
	{
        if (IdentityStatus != Status)
        {
            IdentityStatus = Status;
        }
	}
}

void APlayerStateCustom::SetPlayerBetScore(int32 ScoreValue)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        PlayerBetScore = ScoreValue;
    }
}

void APlayerStateCustom::PrintHandsCardsInfo(FString Text)
{
    UE_LOG(LogTemp, Warning, TEXT("HandsCardsInfo --- %s"), *Text);
    for (auto Elem : HandCards)
    {
        URedTenCardFunctionLibrary::PrintCardInfo(Elem);
    }
}

void APlayerStateCustom::PrintSelectedCardsInfo(FString Text)
{
    UE_LOG(LogTemp, Warning, TEXT("SelectedCardsInfo --- %s"), *Text);
    for (auto Elem : HandCards)
    {
        if (Elem.bSelected)
        {
			URedTenCardFunctionLibrary::PrintCardInfo(Elem);
		}
	}
}

void APlayerStateCustom::OnRep_PlayerIndex()
{
    NotifyPlayerInfoToMainMenuBase();
}

void APlayerStateCustom::OnRep_PlayerCustomName()
{
    NotifyPlayerInfoToMainMenuBase();
}

void APlayerStateCustom::OnRep_PlayerReady()
{
    NotifyPlayerReadyToMainMenuBase();
}

void APlayerStateCustom::OnRep_HandCards()
{
    NotifyPlayerCardsToMainMenuBase();
}

void APlayerStateCustom::OnRep_HandCardsCount()
{
    NotifyPlayerCardsCountToMainMenuBase();
}

void APlayerStateCustom::OnRep_TeamIDChange()
{
    NotifyPlayerTeamIdToMainMenuBase();
}

void APlayerStateCustom::OnRep_BetScoreChange()
{
    NotifyPlayerBetScoreChange();
}

void APlayerStateCustom::OnRep_IdentityStatusChange()
{
    NotifyPlayerIdentityStatusToMenuBase();
}

void APlayerStateCustom::OnRep_GameOverTypeChange()
{
    NotifyPlayerGameOverTypeChange();
}

void APlayerStateCustom::NotifyPlayerJoinMainMenuBase()
{
	ACardPlayerController* PC = Cast<ACardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC)
    {
		PC->OnPlayerJoinRoom(this);
	}
}

void APlayerStateCustom::NotifyPlayerInfoToMainMenuBase()
{
    ACardPlayerController* PC = Cast<ACardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC)
    {
        PC->OnPlayerInfoReceived(this);
    }
}

void APlayerStateCustom::NotifyPlayerReadyToMainMenuBase()
{
	ACardPlayerController* PC = Cast<ACardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->OnPlayerReadyReceived(this);
	}
}

void APlayerStateCustom::NotifyPlayerCardsToMainMenuBase()
{
	ACardPlayerController* PC = Cast<ACardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->OnPlayerCardsReceived(this);
	}
}

void APlayerStateCustom::NotifyPlayerCardsCountToMainMenuBase()
{
    ACardPlayerController* PC = Cast<ACardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC)
    {
        PC->OnUpdateCardCountReceived(this);
    }
}

void APlayerStateCustom::NotifyPlayerTeamIdToMainMenuBase()
{
    ACardPlayerController* PC = Cast<ACardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC)
    {
        PC->OnTeamIdReceived(this);
    }
}

void APlayerStateCustom::NotifyPlayerIdentityStatusToMenuBase()
{
    ACardPlayerController* PC = Cast<ACardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC)
    {
		PC->OnPlayerIdentityUpdate(IdentityStatus);
	}
}

void APlayerStateCustom::NotifyPlayerBetScoreChange()
{
    ACardPlayerController* PC = Cast<ACardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC)
    {
        PC->OnPlayerScoreChange(PlayerBetScore);
    }
}

void APlayerStateCustom::NotifyPlayerGameOverTypeChange()
{
    ACardPlayerController* PC = Cast<ACardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC)
    {
        PC->OnPlayerGameOverChange(PlayerIndex, GameOverType);
    }
}
