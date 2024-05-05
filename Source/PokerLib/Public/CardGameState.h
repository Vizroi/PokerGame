// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerTeamInfo.h"
#include "GrabWind.h"
#include "Deck.h"
#include "GameFramework/GameStateBase.h"
#include "CardGameState.generated.h"

static int32 DefaultGameScore = 10;

class APlayerStateCustom;

UCLASS()
class POKERLIB_API ACardGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACardGameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ResetCardGameStateData();

	void ChangeGamePhase(EGamePhase NewGameState);

public:
	void NotifyLastPlayCardSetChange();
	void NotifyCurrentPlayerIndexChange();
	void NotifyCurAskWindPlayerIndexChange(int32 CurAskWindPlayerIdx);
	void NotifyWindResult(int32 PlayerIdx, EWindResultType Type);
public:
	//get player custom satete by index
	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	APlayerStateCustom* GetPlayerStateByIndex(int32 index);

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	TArray<APlayerStateCustom*> GetPlayerStateArr() {return PlayerStateArray;}

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	void AddPlayerStateArray(APlayerStateCustom* NewPlayerState);

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	void RemovePlayerStateArray(APlayerStateCustom* PlayerState);

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	int32 GetPlayerStateReadyCount();

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	void DealCardToPlayer(UDeck* CardDeck);

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	EGamePhase GetCurrentGamePhase() { return CurrentGamePhase; }

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	void SetGameEnd(bool IsEnd);

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	bool GetGameEnd() const { return IsGameEnd; }

	UFUNCTION(BlueprintCallable, Category = "Team Action")
	void AssignTeam();

	UFUNCTION(BlueprintCallable, Category = "Team Action")
	void RevealAllIdentiy();

	UFUNCTION(BlueprintCallable, Category = "Team Action")
	bool CheckAllPlayerIdentityStatusValid();

	UFUNCTION(BlueprintCallable, Category = "Game Score")
	void MultiplyGameScore();

	UFUNCTION(BlueprintCallable, Category = "Game Score")
	void ResetGameScore();

	UFUNCTION(BlueprintCallable, Category = "Game Score")
	void AddGameScore(int32 Score);

	UFUNCTION(BlueprintCallable, Category = "Game Score")
	int32 GetGameScore();

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void SetCurrentPlayerIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	int32 GetCurrentPlayerIndex() {return CurrentPlayerIndex;}

	//last play cards palyer 
	UFUNCTION(BlueprintCallable, Category = "Player Info")
	int32 GetLastPlayerIndex() { return LastPlayerIndex; }

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	int32 GetLastPlayCardsPlayerIndex() { return LastPlayCardsPlayerIndex; }

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	int32 GetNextCanPlayCardsPlayer(int32 CurPlayerIdx);

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void MoveToNextPlayer();

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void SetPlayerIndexGameOverType(int32 InPlayerIndex, EGameOverType Type);

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	EGameOverType GetPlayerIndexGameOverType(int32 InPlayerIndex);

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	ETeamID GetPlayerIndexTeamID(int32 InPlayerIndex);

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	int32 GetLeftPlayerCount();

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	int32 GetFinishPlayerCount();

	UFUNCTION(BlueprintCallable, Category = "Last Cards Set")
	void AddLastCardSet(int32 PlayerIndex, TArray<FCard> LastCards);

	UFUNCTION(BlueprintCallable, Category = "Last Cards Set")
	TArray<FLastCardSet>& GetLastCardSet() {return PlayerLastCards; }

	UFUNCTION(BlueprintCallable, Category = "Last Cards Set")
	TArray<FCard> GetLastCardSetByPlayerIndex(int32 PlayerIndex);

	UFUNCTION(BlueprintCallable, Category = "Last Cards Set")
	void EmptyLastCardSetByLastPlayCards();

	UFUNCTION(BlueprintCallable, Category = "Grab Wind Info")
	int32 GetNextAskWindPlayerIdx(int32 CurPlayerIdx);

	//询问下一个是否抢风的玩家
	UFUNCTION(BlueprintCallable, Category = "Grab Wind Info")
	void AskNextPlayerGrabWind();

	UFUNCTION(BlueprintCallable, Category = "Grab Wind Info")
	void UpdateWindActionInfo(FWindActionInfo WindActionInfo);

	UFUNCTION(BlueprintCallable, Category = "Grab Wind Info")
	void AddWindActionInfo(FWindActionInfo WindActionInfo);

	UFUNCTION(BlueprintCallable, Category = "Grab Wind Info")
	void HandleFirstPlayerWindAction(FWindActionInfo WindActionInfo);

	UFUNCTION(BlueprintCallable, Category = "Grab Wind Info")
	void HandleSubsequentPlayerWindAction(FWindActionInfo WindActionInfo);

public:
	//通过位运算的变量来知道当前哪个位置是空的
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CardGameState PlayerIndex")
	uint8 AvailableIndices = 0xF;

public:
	UFUNCTION()
	void OnRep_GamePhaseChange();

	UFUNCTION()
	void OnRep_PlayerStateArrayChange();

	UFUNCTION()
	void OnRep_RevealedTeamInfo();

	UFUNCTION()
	void OnRep_GameScoreChange();

	//UFUNCTION()
	//void OnRep_CurrentPlayerIndexChange();

	//UFUNCTION()
	//void OnRep_PlayerLastCardsChange();

	//UFUNCTION()
	//void OnRep_CuirAskWindPlayerIndexChange();
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerStateArrayChange)
	TArray<APlayerStateCustom*> PlayerStateArray;

	UPROPERTY(ReplicatedUsing = OnRep_RevealedTeamInfo)
	TArray<FPlayerTeamInfo> RevealedPlayers;

	UPROPERTY(ReplicatedUsing = OnRep_GameScoreChange)
	int32 GameScore = 0;

	UPROPERTY(ReplicatedUsing = OnRep_GamePhaseChange)
	EGamePhase  CurrentGamePhase = EGamePhase::WaitingForPlayers;

	UPROPERTY(Replicated)
	int32 CurrentPlayerIndex; // Current can play cards index

	UPROPERTY(Replicated)
	int32 LastPlayerIndex = -1;

	UPROPERTY(Replicated)
	int32 LastPlayCardsPlayerIndex = -1;

	UPROPERTY(Replicated)
	TArray<FLastCardSet> PlayerLastCards;

	UPROPERTY(Replicated)
	int32 CurrentAskWindPlayerIndex = -1;

	UPROPERTY(Replicated)
	TArray<FWindActionInfo> WindActionInfoArray;

	UPROPERTY(Replicated)
	bool IsGameEnd = false;
};
