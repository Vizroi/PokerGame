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
    DOREPLIFETIME(APlayerStateCustom, HandCards);
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
    }
}

int32 APlayerStateCustom::GetSeatIndexByPlayerIndex(int32 Index)
{
    int32 SeatIndex = -1;
    APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(GetWorld()->GetFirstPlayerController()->PlayerState);
    if (PlayerState)
    {
        int32 CurrentPlayerIndex = PlayerState->PlayerIndex;
        //if (CurrentPlayerIndex - Index < 0)
        //{
        //    SeatIndex = CurrentPlayerIndex - Index + 4;
        //}
        //else 
        //{
        //    SeatIndex = CurrentPlayerIndex - Index;
        //}
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
