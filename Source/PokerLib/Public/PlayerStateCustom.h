// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateCustom.generated.h"

UCLASS()
class POKERLIB_API APlayerStateCustom : public APlayerState
{   
	GENERATED_BODY()
	
public:
    APlayerStateCustom();
protected:
    // 网络复制相关
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    //set player index in server form gamemode post login
    UFUNCTION(BlueprintCallable, Category = "Player Info")
    void SetPlayerIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Player Info")
    int32 GetPlayerIndex() const { return PlayerIndex; }

    //player custom self name 
    UFUNCTION(BlueprintCallable, Category = "Player Info")
    void SetCustomPlayerName(const FString& NewName);

    UFUNCTION(BlueprintCallable, Category = "Player Info")
	FString GetCustomPlayerName() const { return PlayerCustomName; }

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void SetPlayerReady(bool value);

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    bool GetPlayerReady() const { return bIsReady; }

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void AddCardToHand(const FCard& Card);

    //Remove CardToHand
    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void RemoveCardToHand(const FCard& Card);

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void UpdateCardsCount();

    // 通过PlayerIndex获取在当前屏幕的哪个座位
    UFUNCTION(BlueprintCallable, Category = "Player Info")
    int32 GetSeatIndexByPlayerIndex(int32 Index);

protected:
    UFUNCTION()
    void OnRep_PlayerIndex();

    UFUNCTION()
    void OnRep_PlayerCustomName();

    UFUNCTION()
    void OnRep_PlayerReady();

    UFUNCTION()
    void OnRep_HandCards();

    UFUNCTION()
    void OnRep_HandCardsCount();

protected:
    UPROPERTY(ReplicatedUsing = OnRep_PlayerIndex, VisibleAnywhere, BlueprintReadOnly, Category = "Player Status")
    int32 PlayerIndex = -1;

    UPROPERTY(ReplicatedUsing = OnRep_PlayerCustomName, VisibleAnywhere, BlueprintReadWrite, Category = "Player Info")
    FString PlayerCustomName;

    UPROPERTY(ReplicatedUsing = OnRep_PlayerReady, VisibleAnywhere, BlueprintReadOnly, Category = "Player Status")
    bool bIsReady = false;

    UPROPERTY(ReplicatedUsing = OnRep_HandCards, VisibleAnywhere, BlueprintReadWrite, Category = "Card Game")
    TArray<FCard> HandCards;

    //HandCardsCount
    UPROPERTY(ReplicatedUsing = OnRep_HandCardsCount, VisibleAnywhere, BlueprintReadWrite, Category = "Card Game")
    int32 HandCardsCount;

public:
    void NotifyPlayerJoinMainMenuBase();
    void NotifyPlayerInfoToMainMenuBase();
    void NotifyPlayerReadyToMainMenuBase();
    void NotifyPlayerCardsToMainMenuBase();
    void NotifyPlayerCardsCountToMainMenuBase();












// ---------------------------------------------------------------
public:
    // 玩家押注的分数
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Card Game")
    int32 BetScore;

    // 玩家所属的队伍
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Card Game")
    int32 TeamID;
};

