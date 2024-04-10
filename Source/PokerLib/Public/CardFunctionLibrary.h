// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "CardSet.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CardFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class POKERLIB_API UCardFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Card Functions")
	static bool IsJoker(const FCard& Card);

	UFUNCTION(BlueprintCallable, Category = "Card Functions")
	static void PrintCardInfo(const FCard& Card);

	UFUNCTION(BlueprintCallable, Category = "Card Functions")
	static UTexture2D* GetCardTexture(ESuit Suit, ECardValue Value, UDataTable* DataTable);
};
