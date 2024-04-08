// Fill out your copyright notice in the Description page of Project Settings.


#include "CardPlayerController.h"
#include "PlayerStateCustom.h"
#include "CardGameMode.h"

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

void ACardPlayerController::SelectCard(FCard& Card)
{
	if (IsLocalController())
	{
		Card.bSelected = !Card.bSelected;

		if (!GameMenuWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
			return;
		}

		APlayerStateCustom* PlayerStateCustom = Cast<APlayerStateCustom>(PlayerState);
		if (PlayerStateCustom) 
		{
			GameMenuWidget->OnSelectedCard(PlayerStateCustom, Card.CardID);
		}
	}
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

void ACardPlayerController::OnUpdateCardCountReceived(APlayerStateCustom* NewPlayerStat)
{
if (!NewPlayerStat)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: NewPlayerState is NULL!"));
		return;
	}

	if (!GameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ACardPlayerController::OnPlayerIndexReceived: GameMenuWidget is NULL!"));
		return;
	}

	GameMenuWidget->OnUpdateCardCount(NewPlayerStat);

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
	// 在这里可以添加一些额外的验证逻辑
	// 例如，防止玩家在某些游戏状态下更改准备状态
	return true;
}