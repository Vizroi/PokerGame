// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIGameMainBase.h"
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
	// 调用服务器 加入游戏
	UFUNCTION(BlueprintCallable, Category = "Game|RedTen")
	void JoinGame(const FString& ServerAddress);

	// 调用服务器 准备游戏
	UFUNCTION(BlueprintCallable, Category = "Game|RedTen")
	void ClientNotifyReady();
public:
	void OnPlayerJoinRoom(APlayerStateCustom* NewPlayerState);
	void OnPlayerInfoReceived(APlayerStateCustom* NewPlayerState);
	void OnPlayerReadyReceived(APlayerStateCustom* NewPlayerState);
	void OnPlayerCardsReceived(APlayerStateCustom* NewPlayerState);

public:
	UFUNCTION(Client, Reliable)
	void ClientCreateGameMenuUI();
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerReady();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI_GamePlayMenu")
	TSubclassOf<UUIGameMainBase> GameMenuWidgetClass;

protected:
	UPROPERTY()
	UUIGameMainBase* GameMenuWidget;
};