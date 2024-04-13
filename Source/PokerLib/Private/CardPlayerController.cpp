// Fill out your copyright notice in the Description page of Project Settings.


#include "CardPlayerController.h"
#include "PlayerStateCustom.h"
#include "CardGameMode.h"
#include "RedTenCardFunctionLibrary.h"
#include <CardGameState.h>

ACardPlayerController::ACardPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
	LastCards.Empty();
}

void ACardPlayerController::JoinGame(const FString& ServerAddress)
{
	if (IsLocalController()) 
	{
		ClientTravel(ServerAddress, ETravelType::TRAVEL_Absolute);
	}
}

void ACardPlayerController::ClientNotifyReady()
{
	if (IsLocalController())
	{
		ServerSetPlayerReady();
	}
}

void ACardPlayerController::ClientCreateGameMenuUI_Implementation()
{
	if (IsLocalController())
	{
		// 创建游戏菜单
		if (GameMenuWidgetClass)
		{
			if (!GameMenuWidget)
			{
				GameMenuWidget = CreateWidget<UUIGameMainBase>(this, GameMenuWidgetClass);
				if (GameMenuWidget)
				{
					GameMenuWidget->AddToViewport();
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ACardPlayerController::CreateGameMenuWidget: GameMenuWidget is already created!"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::BeginPlay: Not a local controller!"));
	}
}

TArray<FCard> ACardPlayerController::GetCardsByID(const TArray<int32>& CardId)
{
	TArray<FCard> Cards;
	APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
	if (PS)
	{
		for (int32 Id : CardId)
		{
			FCard Card = PS->GetCardByID(Id);
			Cards.Add(Card);
		}

	}
	return Cards;
}

bool ACardPlayerController::IsCardInHand(int32 CardId)
{
	APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
	if (PS)
	{
		FCard Card = PS->GetCardByID(CardId);
		return Card.Value != ECardValue::None;
	}
	return false;
}

bool ACardPlayerController::IsCanPlayCardsInHand()
{
	APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
	if (!PS)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::IsCanPlayCardsInHand: PlayerState is NULL!"));
	}

	TArray<int32> CardsIdArray = PS->GetAllSelectedCardID();

	if (CheckPlayCards(CardsIdArray))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::IsCanPlayCardsInHand: Can't play the cards!"));
		return false;
	}
}

bool ACardPlayerController::CheckPlayCards(const TArray<int32>& CardId)
{
	TArray<FCard> CurCards = GetCardsByID(CardId);
	bool IsCanPlayCard = URedTenCardFunctionLibrary::CanPlayCard(CurCards, LastCards);

	return IsCanPlayCard;
}


void ACardPlayerController::SortHandCards()
{
	if (IsLocalController())
	{
		APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
		if (PS != NULL)
		{
			PS->SortHandCards();
		}
	}
}

bool ACardPlayerController::SelectCard(int32 CardId)
{
	if (IsLocalController())
	{
		APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
		if (PS != NULL)
		{
			bool IsSelected = PS->SelectCardToHand(CardId);
			if (GameMenuWidget)
			{
				GameMenuWidget->OnSelectCard();
			}

			return IsSelected;
		}
	}
	return false;
}

void ACardPlayerController::ClientPlayCards()
{
	if (IsLocalController())
	{
		APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
		if (!PS)
		{
			UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::PlayCards: PlayerState is NULL!"));
		}

		TArray<int32> CardsIdArray = PS->GetAllSelectedCardID();

		if (CheckPlayCards(CardsIdArray))
		{
			ServerPlayCards(CardsIdArray);

			PS->PrintHandsCardsInfo("ClientPlayCards: ");
			PS->PrintSelectedCardsInfo("ClientPlayCards: ");
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::PlayCards: Can't play the cards!"));
		}
	}
}

void ACardPlayerController::ClientRevealAllIdentiy(bool IsReveal)
{
	ServerRevealAllIdentiy(IsReveal);
}

void ACardPlayerController::OnGamePhaseChange(EGamePhase CurGamePhase)
{
	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnGamePhaseChange(CurGamePhase);
}

void ACardPlayerController::OnPlayerJoinRoom(APlayerStateCustom* NewPlayerState)
{
	if (!NewPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: NewPlayerState is NULL!"));
		return;
	}

	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnPlayerJoinRoom(NewPlayerState);
}

void ACardPlayerController::OnPlayerInfoReceived(APlayerStateCustom* NewPlayerState)
{
	if (!NewPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: NewPlayerState is NULL!"));
		return;
	}

	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnPlayerUpdate(NewPlayerState);
}

void ACardPlayerController::OnPlayerReadyReceived(APlayerStateCustom* NewPlayerState)
{
	if (!NewPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: NewPlayerState is NULL!"));
		return;
	}

	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnPlayerReady(NewPlayerState);
}

void ACardPlayerController::OnPlayerCardsReceived(APlayerStateCustom* NewPlayerState)
{
	if (!NewPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: NewPlayerState is NULL!"));
		return;
	}

	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnDealCards(NewPlayerState);
}

void ACardPlayerController::OnUpdateCardCountReceived(APlayerStateCustom* NewPlayerState)
{
	if (!NewPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: NewPlayerState is NULL!"));
		return;
	}

	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnUpdateCardCount(NewPlayerState);
}

void ACardPlayerController::OnTeamIdReceived(APlayerStateCustom* NewPlayerState)
{
	if (!NewPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: NewPlayerState is NULL!"));
		return;
	}

	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnTeamIdReceived(NewPlayerState);
}

void ACardPlayerController::OnUpdateGameScore(int32 GameScore)
{
	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnUpdateGameScore(GameScore);
}

void ACardPlayerController::OnRevealAllIdentity(const TArray<FPlayerTeamInfo>& PlayerTeamInfoArr)
{
	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnRevealAllIdentity(PlayerTeamInfoArr);
}

void ACardPlayerController::OnPlayerIdentityUpdate(EIdentityStatus Status)
{
	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnPlayerIdentityUpdate(Status);

}

void ACardPlayerController::ServerRevealAllIdentiy_Implementation(bool IsReveal)
{
	ACardGameState* GS = Cast<ACardGameState>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->RevealAllIdentiy(IsReveal);
	}
}

bool ACardPlayerController::ServerRevealAllIdentiy_Validate(bool IsReveal)
{
	return true;
}

void ACardPlayerController::ServerSetPlayerReady_Implementation()
{
	APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
	if (!PS)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ServerSetPlayerReady_Implementation: PlayerState is NULL!"));
		return;
	}

	PS->SetPlayerReady(true);

	ACardGameMode* GameMode = Cast<ACardGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnPlayerReady();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ServerSetPlayerReady_Implementation: GameMode is NULL!"));
	}
}

bool ACardPlayerController::ServerSetPlayerReady_Validate()
{
	return true;
}

void ACardPlayerController::ServerPlayCards_Implementation(const TArray<int32>& CardsId)
{
	APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
	if (!PS)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ServerPlayCards: PlayerState is NULL!"));
		return;
	}

	if (!CheckPlayCards(CardsId))
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ServerPlayCards: Can't Try Play Cards"));
		return;
	}
	else
	{
		LastCards.Empty();
		LastCards = GetCardsByID(CardsId);

		PS->PrintHandsCardsInfo("ServerPlayCards: ");
		PS->RemoveCardToHandFormCardId(CardsId);
	}
}

bool ACardPlayerController::ServerPlayCards_Validate(const TArray<int32>& CardsId)
{
	return true;
}