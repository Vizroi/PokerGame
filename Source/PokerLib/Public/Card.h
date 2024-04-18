// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.generated.h"

// 定义扑克牌的花色枚举
UENUM(BlueprintType)
enum class ESuit : uint8
{
	Spade UMETA(DisplayName = "黑桃"), // 黑桃
	Heart UMETA(DisplayName = "红桃"), // 红桃
	Club UMETA(DisplayName = "梅花"), // 梅花
	Diamond UMETA(DisplayName = "方块"), // 方块
	Joker UMETA(DisplayName = "鬼牌") // 鬼牌
};

// 定义扑克牌的值
UENUM(BlueprintType)
enum class ECardValue : uint8
{
	None UMETA(DisplayName = "None"),
	Ace UMETA(DisplayName = "Ace"),
	Two UMETA(DisplayName = "2"),
	Three UMETA(DisplayName = "3"),
	Four UMETA(DisplayName = "4"),
	Five UMETA(DisplayName = "5"),
	Six UMETA(DisplayName = "6"),
	Seven UMETA(DisplayName = "7"),
	Eight UMETA(DisplayName = "8"),
	Nine UMETA(DisplayName = "9"),
	Ten UMETA(DisplayName = "10"),
	J UMETA(DisplayName = "J"),
	Q UMETA(DisplayName = "Q"),
	K UMETA(DisplayName = "K"),
	JokerA = 99 UMETA(DisplayName = "JokerA"),
	JokerB = 100 UMETA(DisplayName = "JokerB")
};

FString GetCardValueString(ECardValue Value)
{
	switch (Value)
	{
	case ECardValue::Ace:
		return "A";
	case ECardValue::J:
		return "J";
	case ECardValue::Q:
		return "Q";
	case ECardValue::K:
		return "K";
	case ECardValue::JokerA:
		return "JokerA";
	case ECardValue::JokerB:
		return "JokerB";
	default:
		return FString::FromInt(static_cast<int32>(Value));
	}
}

FString GetCardSuitString(ESuit Suit)
{
	//switch (Suit)
	//{
	//	case ESuit::Spade:
	//		return "黑桃";
	//	case ESuit::Heart:
	//		return "红桃";
	//	case ESuit::Club:
	//		return "梅花";
	//	case ESuit::Diamond:
	//		return "方片";
	//	case ESuit::Joker:
	//		return "鬼牌";
	//}

	return UEnum::GetValueAsString(Suit);
}

USTRUCT(BlueprintType)
struct FCardDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ESuit Suit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ECardValue Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	TSoftObjectPtr<UTexture2D> CardImage;
};

USTRUCT(BlueprintType)
struct FCard
{
	GENERATED_BODY()

	FCard() : CardID(-1), Suit(ESuit::Spade), Value(ECardValue::None), bSelected(false){}

	FCard(int32 InCardID, ESuit InSuit, ECardValue Invalue, bool InSelected) : CardID(InCardID), Suit(InSuit), Value(Invalue), bSelected(InSelected) {}

	bool operator==(const FCard& Other) const
	{
		return Suit == Other.Suit && Value == Other.Value;
	}

	bool IsEmpty() const
	{
		return CardID == -1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	int32 CardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ESuit Suit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ECardValue Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	bool bSelected = false;
};


USTRUCT(BlueprintType)
struct FLastCardSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 PlayerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<FCard> LastCards;
};