// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GrabWind.generated.h"

UENUM(BlueprintType)
enum class EWindType : uint8
{
	EWT_InValid,
	EWT_WardWind,
	EWT_GrabWind,
	EWT_GiveUpWind,
};

UENUM(BlueprintType)
enum class EWindResultType : uint8
{
	EWRT_InValid,
	EWRT_WardWindSuccess,
	EWRT_WardWindFail,
	EWRT_GrabWindSuccess,
};

USTRUCT(BlueprintType)
struct FWindActionInfo
{
	GENERATED_USTRUCT_BODY()

	FWindActionInfo(): WindType(EWindType::EWT_InValid), PlayerIdx(-1)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GrabWindInfo")
	EWindType WindType = EWindType::EWT_InValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GrabWindInfo")
	int32 PlayerIdx = -1;
};
