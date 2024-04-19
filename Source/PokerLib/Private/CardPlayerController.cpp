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
		UE_LOG(LogTemp, Warning , TEXT("ACardPlayerController::IsCanPlayCardsInHand: Can't play the cards!"));
		return false;
	}
}

bool ACardPlayerController::CheckPlayCards(const TArray<int32>& CardId)
{
	ACardGameState* GS = Cast<ACardGameState>(GetWorld()->GetGameState());
	if (!GS)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::CheckPlayCards: GameState is NULL!"));
		return false;

	}

	if (GS->GetCurrentPlayerIndex() == GS->GetLastPlayerIndex())
	{
		return true;
	}

	TArray<FCard> CurCards = GetCardsByID(CardId);
	TArray<FCard> LastCards = GS->GetLastCardSetByPlayerIndex(GS->GetLastPlayerIndex());
	bool IsCanPlayCard = URedTenCardFunctionLibrary::CompareCards(CurCards, LastCards);

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

void ACardPlayerController::ClientSelectCard(int32 CardId)
{
	if (IsLocalController())
	{
		if (IsCardInHand(CardId))
		{
			ServerSelectCard(CardId);
		}
	}
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
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::PlayCards: Can't play the cards!"));
		}
	}
}

void ACardPlayerController::ClientPassTurn()
{
	if (IsLocalController())
	{
		APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
		if (!PS)
		{
			UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ClientPassTurn: PlayerState is NULL!"));
		}

		TArray<int32> CardsIdArray = PS->GetAllSelectedCardID();
		if (CardsIdArray.Num() < 1)
		{
			ServerPassTurn();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ClientPassTurn: Can't pass the turn!"));
		}
	}
}

void ACardPlayerController::ClientRevealAllIdentiy(bool IsReveal)
{
	ServerRevealAllIdentiy(IsReveal);
}

void ACardPlayerController::ClientUpdateSelectCardToHand_Implementation(int32 CardId, bool IsSelected, bool IsCanPlay)
{
	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ClientUpdateSelectCardToHand: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnSelectCard(CardId, IsSelected, IsCanPlay);
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

void ACardPlayerController::OnCurrentPlayerIndexChange(int32 CurPlayerIndex)
{
	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnCurrentPlayerIndexChange(CurPlayerIndex);
}

void ACardPlayerController::OnPlayerLastCardsChange(int32 PlayerIndex, const TArray<FCard>& Cards)
{
	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerLastCardsChange: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnLastCardsChange(PlayerIndex, Cards);
}

void ACardPlayerController::ServerRevealAllIdentiy_Implementation(bool IsReveal)
{
	APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
	if (PS)
	{
		if (IsReveal)
		{
			PS->SetIdentityStatus(EIdentityStatus::Identity);
		}
		else
		{
			PS->SetIdentityStatus(EIdentityStatus::NoIdentity);
		}
	}

	ACardGameState* GS = Cast<ACardGameState>(GetWorld()->GetGameState());
	if (GS)
	{
		if (IsReveal)
		{
			GS->RevealAllIdentiy();
		}

		bool IsAllPlayerStatusValid = GS->CheckAllPlayerIdentityStatusValid();
		if (IsAllPlayerStatusValid)
		{
			GS->MoveToNextPlayer();
			GS->ChangeGamePhase(EGamePhase::Playing);
		}
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

void ACardPlayerController::ServerSelectCard_Implementation(int32 CardId)
{
	APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
	if (PS != NULL)
	{
		bool IsSelect = PS->SelectCardToHand(CardId);
		bool IsCanPlay = IsCanPlayCardsInHand();
		ClientUpdateSelectCardToHand(CardId, IsSelect, IsCanPlay);
		PS->PrintSelectedCardsInfo("ServerSelectCard : ");
	}
}

bool ACardPlayerController::ServerSelectCard_Validate(int32 CardId)
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

	ACardGameState* GS = Cast<ACardGameState>(GetWorld()->GetGameState());
	if (!GS)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ServerPlayCards: GameState is NULL!"));
		return;
	
	}

	if (!CheckPlayCards(CardsId))
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ServerPlayCards: Can't Try Play Cards"));
		return;
	}
	else
	{
		TArray<FCard> LastCards = GetCardsByID(CardsId);
		GS->AddLastCardSet(PS->GetPlayerIndex(), LastCards);


		PS->PrintHandsCardsInfo("ServerPlayCards: ");
		PS->RemoveCardToHandFormCardId(CardsId);

		GS->MoveToNextPlayer();
	}
}

bool ACardPlayerController::ServerPlayCards_Validate(const TArray<int32>& CardsId)
{
	return true;
}

void ACardPlayerController::ServerPassTurn_Implementation()
{
	APlayerStateCustom* PS = Cast<APlayerStateCustom>(PlayerState);
	if (!PS)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ServerPassTurn: PlayerState is NULL!"));
		return;
	}

	ACardGameState* GS = Cast<ACardGameState>(GetWorld()->GetGameState());
	if (!GS)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::ServerPassTurn: GameState is NULL!"));
		return;

	}

	TArray<FCard> LastCards;
	GS->AddLastCardSet(PS->GetPlayerIndex(), LastCards);
	PS->PrintHandsCardsInfo("ServerPassTurn: ");
	GS->MoveToNextPlayer();
}

bool ACardPlayerController::ServerPassTurn_Validate()
{
	return true;
}