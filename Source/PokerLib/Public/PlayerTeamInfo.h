// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerTeamInfo.generated.h"

/**
 * 
 */

class APlayerStateCustom;

UENUM(BlueprintType)
enum class ETeamID : uint8
{
    InValid UMETA(DisplayName = "InValid"),
    TwoPairRedTen UMETA(DisplayName = "TWoPairRedTen"),
    RedTen_Heart UMETA(DisplayName = "RedTen_Heart"),
    RedTen_Diamond UMETA(DisplayName = "RedTen_Diamond"),
    NoRedTen UMETA(DisplayName = "NoRedTen")
};

UENUM(BlueprintType)
enum class EIdentityStatus : uint8
{
    InValid UMETA(DisplayName = "InValid"),
    Identity UMETA(DisplayName = "Identity"),
    NoIdentity UMETA(DisplayName = "NoIdentity")
};

UENUM(BlueprintType)
enum class EGameOverType : uint8
{
    None UMETA(DisplayName = "None"),
    Win UMETA(DisplayName = "Win"),
    Lose UMETA(DisplayName = "Lose"),
    Draw UMETA(DisplayName = "Draw")
};

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
    WaitingForPlayers,
    DealingCards,
    AssigningTeams,
    RevealingTeams,
    Playing,
    WindAction,
    GameOver
};

USTRUCT(BlueprintType)
struct FPlayerTeamInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    APlayerStateCustom* PlayerState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    ETeamID TeamID;
};