// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateCustom.generated.h"

UENUM(BlueprintType)
enum class ETeamID : uint8
{
    InValid UMETA(DisplayName = "InValid"),
	RedTen UMETA(DisplayName = "RedTen"),
	NoRedTen UMETA(DisplayName = "NoRedTen")
};

UENUM(BlueprintType)
enum class EIdentityStatus : uint8
{
    InValid UMETA(DisplayName = "InValid"),
	Identity UMETA(DisplayName = "Identity"),
	NoIdentity UMETA(DisplayName = "NoIdentity")
};


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

    UFUNCTION(BlueprintCallable, Category = "Player Info")
    void SetPlayerReady(bool value);

    UFUNCTION(BlueprintCallable, Category = "Player Info")
    bool GetPlayerReady() const { return bIsReady; }

    UFUNCTION(BlueprintCallable, Category = "Card Game")
	void ClearHandCards();

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void AddCardToHand(const FCard& Card);

    //Remove CardToHand
    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void RemoveCardToHand(const FCard& Card);

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void RemoveCardToHandFormCardId(const TArray<int32>& CardId);

    //sort hand cads
    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void SortHandCards();

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    bool SelectCardToHand(int32 CardId);

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void ClearSelectedCards();

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    TArray<int32> GetAllCardID();

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    TArray<int32> GetAllSelectedCardID();

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    FCard GetCardByID(int32 CardID);

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void UpdateCardsCount();

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    bool HasCard(ESuit Suit, ECardValue Value);

    UFUNCTION(BlueprintCallable, Category = "Card Game")
    void HasReadTen(bool& HasHeartTen, bool& HasDiamondTen);

    // 通过PlayerIndex获取在当前屏幕的哪个座位
    UFUNCTION(BlueprintCallable, Category = "Player Info")
    int32 GetSeatIndexByPlayerIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Team Info")
    void AssignTeamID();

    UFUNCTION(BlueprintCallable, Category = "Team Info")
	ETeamID GetTeamID() const { return TeamID; }

    UFUNCTION(BlueprintCallable, Category = "Team Info")
    void SetIdentityStatus(EIdentityStatus Status);

    UFUNCTION(BlueprintCallable, Category = "Team Info")
    EIdentityStatus GetIdentityStatus() { return IdentityStatus; }
    

    UFUNCTION(BlueprintCallable, Category = "Score Info")
    void SetPlayerBetScore(int32 ScoreValue);
public:
    //输出当前手里的卡牌信息
    UFUNCTION(BlueprintCallable, Category = "Debug Card Info")
    void PrintHandsCardsInfo(FString Text);

    //输出当前手里被选中的卡牌信息
    UFUNCTION(BlueprintCallable, Category = "Debug Card Info")
    void PrintSelectedCardsInfo(FString Text);

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

    UFUNCTION()
    void OnRep_TeamIDChange();

    UFUNCTION()
    void OnRep_IdentityStatusChange();

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

    UPROPERTY(ReplicatedUsing = OnRep_TeamIDChange, VisibleAnywhere, BlueprintReadWrite, Category = "Card Game")
    ETeamID TeamID;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Card Game")
    int32 PlayerBetScore;

    UPROPERTY(ReplicatedUsing = OnRep_IdentityStatusChange, VisibleAnywhere, BlueprintReadWrite, Category = "Player Status")
    EIdentityStatus IdentityStatus = EIdentityStatus::InValid;

public:
    void NotifyPlayerJoinMainMenuBase();
    void NotifyPlayerInfoToMainMenuBase();
    void NotifyPlayerReadyToMainMenuBase();
    void NotifyPlayerCardsToMainMenuBase();
    void NotifyPlayerCardsCountToMainMenuBase();
    void NotifyPlayerTeamIdToMainMenuBase();
    void NotifyPlayerIdentityStatusToMenuBase();
};

