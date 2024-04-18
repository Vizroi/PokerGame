// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Card.h"
#include "CoreMinimal.h"
#include "CardSet.generated.h"

/**
 * ��Ҫ�����Ƶ��������,�������жϳ��Ƶ��߼�
 */

UENUM(BlueprintType)
enum class ECardSetType : uint8
{
	Empty,
	Single,
	Pair,
	Bomb,
	BigBomb,
	Straight,
	RedTens,
	Invalid
};


USTRUCT(BlueprintType)
struct FCardSet
{
	GENERATED_BODY()

	TArray<FCard> Cards;
	ECardSetType Type = ECardSetType::Invalid;
	FCard HighestCard = FCard();
};
