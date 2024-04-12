// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateCustom.h"
#include "PlayerTeamInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerTeamInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    APlayerStateCustom* PlayerState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    ETeamID TeamID;
};