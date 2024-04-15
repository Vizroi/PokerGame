// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Deck.h"
#include "PlayerStateCustom.h"
#include "PlayerTeamInfo.h"
#include "CardGameState.generated.h"

int32 DefaultGameScore = 10;

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	WaitingForPlayers,
	DealingCards,
	AssigningTeams,
	RevealingTeams,
	Playing,
	GameOver
};


UCLASS()
class POKERLIB_API ACardGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACardGameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	void ChangeGamePhase(EGamePhase NewGameState);

public:
	//get player custom satete by index
	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	APlayerStateCustom* GetPlayerStateByIndex(int32 index);

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	void AddPlayerStateArray(APlayerStateCustom* NewPlayerState);

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	void RemovePlayerStateArray(APlayerStateCustom* PlayerState);

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	int32 GetPlayerStateReadyCount();

	UFUNCTION(BlueprintCallable, Category = "CardGameState")
	void DealCardToPlayer(UDeck* CardDeck);

	UFUNCTION(BlueprintCallable, Category = "Team Action")
	void AssignTeam();

	UFUNCTION(BlueprintCallable, Category = "Team Action")
	void RevealAllIdentiy();


	UFUNCTION(BlueprintCallable, Category = "Game Score")
	void MultiplyGameScore();

	UFUNCTION(BlueprintCallable, Category = "Game Score")
	void ResetGameScore();

	UFUNCTION(BlueprintCallable, Category = "Game Score")
	void AddGameScore(int32 Score);

	UFUNCTION(BlueprintCallable, Category = "Game Score")
	int32 GetGameScore();

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
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerStateArrayChange)
	TArray<APlayerStateCustom*> PlayerStateArray;

	UPROPERTY(ReplicatedUsing = OnRep_RevealedTeamInfo)
	TArray<FPlayerTeamInfo> RevealedPlayers;

	UPROPERTY(ReplicatedUsing = OnRep_GameScoreChange)
	int32 GameScore = 0;

	UPROPERTY(ReplicatedUsing = OnRep_GamePhaseChange)
	EGamePhase  CurrentGamePhase = EGamePhase::WaitingForPlayers;
};
