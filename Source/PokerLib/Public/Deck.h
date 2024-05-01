// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "UObject/NoExportTypes.h"
#include "Deck.generated.h"

UENUM(BlueprintType)
enum class EDeckMode : uint8
{
    Standard,
    RedTen,
    TexasHoldem,
};

UCLASS()
class POKERLIB_API UDeck : public UObject
{
	GENERATED_BODY()
public:
    UDeck();

    // 初始化卡组
    void InitializeDeck(EDeckMode Mode);

    // 洗牌
    void ShuffleDeck();

    // 发一张牌
    FCard DealOneCard();

    // 检查卡组是否为空
    bool IsEmpty() const;

protected:
    TArray<FCard> Cards;
};
