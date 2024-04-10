// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateCustom.h"
#include "Net/UnrealNetwork.h"
#include "CardPlayerController.h"
#include <Kismet/GameplayStatics.h>

APlayerStateCustom::APlayerStateCustom()
	:BetScore(0),
	TeamID(-1)
{
	bReplicates = true;
}

void APlayerStateCustom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APlayerStateCustom, PlayerIndex);
    DOREPLIFETIME(APlayerStateCustom, PlayerCustomName);
    DOREPLIFETIME(APlayerStateCustom, bIsReady); 
    DOREPLIFETIME(APlayerStateCustom, HandCardsCount);
    DOREPLIFETIME_CONDITION(APlayerStateCustom, HandCards, COND_OwnerOnly);
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
		for(int32 i = 0; i < CardId.Num(); i++)
		{
			for(int32 j = 0; j < HandCards.Num(); j++)
			{
				if (HandCards[j].CardID == CardId[i])
				{
					HandCards.RemoveAt(j);
					break;
				}
			}
		}
		UpdateCardsCount();
	}
}

bool APlayerStateCustom::SelectCardToHand(int32 CardId)
{
    bool IsSelected = false;
    for (FCard& Card : HandCards)
    {
        if (Card.CardID == CardId)
        {
            Card.bSelected = !Card.bSelected;
            IsSelected = Card.bSelected;
            break;
        }
    }
    return IsSelected;
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
