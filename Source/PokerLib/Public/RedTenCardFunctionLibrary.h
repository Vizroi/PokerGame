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
	static bool CompareCards(const TArray<FCard>& CurrentCards, const TArray<FCard>& LastCards);

public:

	//check is two pair of red ten
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsTwoPairOfRedTen(const TArray<FCard>& Cards);

	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsRedTen(const FCard& Card);

	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsCardsUniformSet(const TArray<FCard>& Cards, int32 RequiredBombCount);

	//check is bigbomb
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsBigBomb(const TArray<FCard>& Cards);
	//check is bomb
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsBomb(const TArray<FCard>& Cards);

	//check is pair
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsPair(const TArray<FCard>& Cards);

	//check is straight
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsStraight(const TArray<FCard>& Cards);

	//check is single
	UFUNCTION(BlueprintCallable, Category = "CardSet Check Result")
	static bool IsSingle(const TArray<FCard>& Cards);

	UFUNCTION(BlueprintCallable, Category = "Cards Priority")
	static FCard GetCardHighestCard(const TArray<FCard>& Cards);

	UFUNCTION(BlueprintCallable, Category = "Cards Priority")
	static FCard GetStraightHighestCard(const TArray<FCard>& Cards);

	UFUNCTION(BlueprintCallable, Category = "Cards Priority")
	static int32 GetCardPriority(const FCard& Card);

	UFUNCTION(BlueprintCallable, Category = "Cards Priority")
	static int32 GetCardStraightPriority(const FCard& Card);

	UFUNCTION(BlueprintCallable, Category = "Cards Priority")
	static ECardValue GetCardValueForStraightPriority(int32 PriorityValue);

	UFUNCTION(BlueprintCallable, Category = "Cards Priority")
	static bool CompareCardsPriority(const FCard& Card1, const FCard& Card2);

	UFUNCTION(BlueprintCallable, Category = "Cards Sort")
	static int32 SortCardsForPriority(TArray<FCard>& Cards);

	UFUNCTION(BlueprintCallable, Category = "Cards Sort")
	static int32 SortCardsForStraightPriority(TArray<FCard>& Cards);



};
