// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "Deck.h"
#include "CardPlayerController.h"
#include "PlayerStateCustom.h"
#include "GameFramework/GameModeBase.h"
#include "CardGameMode.generated.h"

/**
 * 
 */
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

protected:
    UPROPERTY()
    UDeck* GameDeck = nullptr;

private:
    bool bGameStarted = false;
};
 