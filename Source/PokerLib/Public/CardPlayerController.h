// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIGameMainBase.h"
#include "CardSet.cpp"
#include "GameFramework/PlayerController.h"
#include "CardPlayerController.generated.h"

/**
 * 
 */


UCLASS()
class POKERLIB_API ACardPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACardPlayerController();
public:
	UFUNCTION(BlueprintCallable, Category = "Game|RedTen")
	void JoinGame(const FString& ServerAddress);

	UFUNCTION(BlueprintCallable, Category = "Game|RedTen")
	void ClientNotifyReady();

	UFUNCTION(Client, Reliable)
	void ClientCreateGameMenuUI();

public:
	//根据卡牌ID获取卡牌
	UFUNCTION(BlueprintCallable, Category = "Game|Card Info")
	TArray<FCard> GetCardsByID(const TArray<int32>& CardId);

	UFUNCTION(BlueprintCallable, Category = "Game|Card Info")
	bool IsCardInHand(int32 CardId);

	//根据手里的牌判断是否可以出牌
	UFUNCTION(BlueprintCallable, Category = "Game|Card Info")
	bool IsCanPlayCardsInHand();

	UFUNCTION(BlueprintCallable, Category = "Game|Card Info")
	bool CheckPlayCards(const TArray<int32>& CardId);

public:
	UFUNCTION(BlueprintCallable, Category = "Game|Card Action")
	void SortHandCards();

	UFUNCTION(BlueprintCallable, Category = "Game|Card Action")
	bool SelectCard(int32 CardId);

	UFUNCTION(BlueprintCallable, Category = "Game|Card Action")
	void ClientPlayCards();

public:


public:
	void OnPlayerJoinRoom(APlayerStateCustom* NewPlayerState);
	void OnPlayerInfoReceived(APlayerStateCustom* NewPlayerState);
	void OnPlayerReadyReceived(APlayerStateCustom* NewPlayerState);
	void OnPlayerCardsReceived(APlayerStateCustom* NewPlayerState);
	void OnUpdateCardCountReceived(APlayerStateCustom* NewPlayerStat);

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerReady();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlayCards(const TArray<int32>& CardsId);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI_GamePlayMenu")
	TSubclassOf<UUIGameMainBase> GameMenuWidgetClass;

private:
	UPROPERTY()
	UUIGameMainBase* GameMenuWidget;

	UPROPERTY()
	TArray<FCard> LastCards;
};