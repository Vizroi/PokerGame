// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateCustom.h"
#include "UIGameMainBase.generated.h"

/**
 * 游戏游玩主要界面
 */
UCLASS()
class POKERLIB_API UUIGameMainBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//玩家加入房间
	UFUNCTION(BlueprintImplementableEvent, Category = "UIGameMainBase")
		void OnPlayerJoinRoom(APlayerStateCustom* PlayerState);

	//玩家更新
	UFUNCTION(BlueprintImplementableEvent, Category = "UIGameMainBase")
		void OnPlayerUpdate(APlayerStateCustom* PlayerState);

	//玩家进入准备状态
	UFUNCTION(BlueprintImplementableEvent, Category = "UIGameMainBase")
		void OnPlayerReady(APlayerStateCustom* PlayerState);


	//给玩家发牌
	UFUNCTION(BlueprintImplementableEvent, Category = "UIGameMainBase")
		void OnDealCards(APlayerStateCustom* PlayerState);

	//更新牌数量
	UFUNCTION(BlueprintImplementableEvent, Category = "UIGameMainBase")
		void OnUpdateCardCount(APlayerStateCustom* PlayerState);

	//当玩家队伍更新
	UFUNCTION(BlueprintImplementableEvent, Category = "UIGameMainBase")
		void OnTeamIdReceived(APlayerStateCustom* PlayerState);

	UFUNCTION(BlueprintImplementableEvent, Category = "UIGameMainBase")
		void OnUpdateGameScore(int32 GameScore);

	UFUNCTION(BlueprintImplementableEvent, Category = "UIGameMainBase")
		void OnRevealAllIdentity(const TArray<FPlayerTeamInfo>& PlayerTeamInfoArr);

	//当卡牌被点击
	UFUNCTION(BlueprintImplementableEvent, Category = "UIGameMainBase")
		void OnSelectCard();

};
