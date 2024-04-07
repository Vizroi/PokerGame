// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "Card.generated.h"

// 定义扑克牌的花色枚举
UENUM(BlueprintType)
enum class ESuit : uint8
{
	Spade UMETA(DisplayName = "Spade"), // 黑桃
	Heart UMETA(DisplayName = "Heart"), // 红桃
	Club UMETA(DisplayName = "Club"), // 梅花
	Diamond UMETA(DisplayName = "Diamond"), // 方块
	Joker UMETA(DisplayName = "Joker") // 鬼牌
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
	JokerA UMETA(DisplayName = "JokerA"),
	JokerB UMETA(DisplayName = "JokerB")
};

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

	FCard() : Suit(ESuit::Spade), Value(ECardValue::None) {}

	FCard(ESuit InSuit, ECardValue Invalue) : Suit(InSuit), Value(Invalue) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ESuit Suit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ECardValue Value;
};


