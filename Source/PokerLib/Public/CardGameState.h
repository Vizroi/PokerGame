// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Deck.h"
#include "PlayerStateCustom.h"
#include "CardGameState.generated.h"


UCLASS()
class POKERLIB_API ACardGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACardGameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	void RevealIdentiy(APlayerStateCustom* PSC);

public:
	//通过位运算的变量来知道当前哪个位置是空的
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CardGameState PlayerIndex")
	uint8 AvailableIndices = 0xF;

public:
	UFUNCTION()
	void OnRep_PlayerStateArrayChange();

	UFUNCTION()
	void OnRep_AssignTeam();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerStateArrayChange)
	TArray<APlayerStateCustom*> PlayerStateArray;

	UPROPERTY(ReplicatedUsing = OnRep_AssignTeam)
	TArray<APlayerStateCustom*> TeamRedTen;
};
