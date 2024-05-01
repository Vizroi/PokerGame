// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Deck.h"
#include "PlayerTeamInfo.h"
#include "GameFramework/GameModeBase.h"
#include "CardGameMode.generated.h"

UCLASS()
class POKERLIB_API ACardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    ACardGameMode();

    //login and logout
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

    void OnPlayerReady();
    void StartGame();
    void ReStartGame();
    void InitializeCards(EDeckMode Mode);
    void DealCardsToPlayers();

    //AssignTeams
    void AssignTeams();
    void ReSetGameScore();

    //check game is over
    void OnPlayerFinishPlayCards(int32 PlayerIndex);
    bool CheckGameOver();
    void EndGame();

    //ward or grab wind 
    void CheckWardOrGrabWind();

protected:
    UPROPERTY()
    UDeck* GameDeck = nullptr;

    UPROPERTY()
    TArray<int32> FinshiPlayers;

private:
    bool bGameStarted = false;
};
 