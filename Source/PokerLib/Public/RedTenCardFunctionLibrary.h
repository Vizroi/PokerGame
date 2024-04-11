// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "CardSet.h"
#include "CardFunctionLibrary.h"
#include "RedTenCardFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class POKERLIB_API URedTenCardFunctionLibrary : public UCardFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CardSet Functions")
	static FCardSet DetermineCardSetType(const TArray<FCard>& Cards);

	UFUNCTION(BlueprintCallable, Category = "CardSet Functions")
	static bool CanPlayCard(const TArray<FCard>& CurrentCards, const TArray<FCard>& LastCards);

public:
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static void CountCards(const TArray<FCard>& Cards, TMap<ECardValue, int32>& CardCounts, int32& JokerCount);

	//check is two pair of red ten
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsTwoPairOfRedTen(const TArray<FCard>& Cards);

	//check is bigbomb
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsBigBomb(const TArray<FCard>& Cards, ECardValue& HightestValue);
	//check is bomb
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsBomb(const TArray<FCard>& Cards, ECardValue& HightestValue);

	//check is pair
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsPair(const TArray<FCard>& Cards, ECardValue& HightestValue);

	//check is straight
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsStraight(const TArray<FCard>& Cards, ECardValue& HightestValue);

	//check is single
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsSingle(const TArray<FCard>& Cards, ECardValue& HightestValue);

	//get card priority
	UFUNCTION(BlueprintCallable, Category = "Cards Priority")
	static int32 GetCardPriority(const FCard& Card);

	//sort cards
	UFUNCTION(BlueprintCallable, Category = "Cards Sort")
	static int32 SortCards(TArray<FCard>& Cards);

private:
	bool CustomRedTenCardSort(const FCard& A, const FCard& B);
};
