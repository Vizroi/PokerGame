// Fill out your copyright notice in the Description page of Project Settings.


#include "CardFunctionLibrary.h"

bool UCardFunctionLibrary::IsJoker(const FCard& Card)
{
    return Card.Suit == ESuit::Joker || Card.Value == ECardValue::JokerA || Card.Value == ECardValue::JokerB;
}

void UCardFunctionLibrary::PrintCardInfo(const FCard& Card, FString AppendString)
{
    FString SuitString = GetCardSuitString(Card.Suit);
    FString ValueString = GetCardValueString(Card.Value);

    UE_LOG(LogTemp, Warning, TEXT("%s Card Info: %s of %s"), *AppendString , *SuitString , *ValueString);
}

UTexture2D* UCardFunctionLibrary::GetCardTexture(ESuit Suit, ECardValue Value, UDataTable* DataTable)
{
    if (!DataTable)
    {
        return nullptr;
    }

    static const FString ContextString(TEXT("Card lookup"));
    for (auto& Row : DataTable->GetRowMap())
    {
        FCardDataTable* CardData = (FCardDataTable*)Row.Value;
        if (CardData && CardData->Suit == Suit && CardData->Value == Value)
        {
            return CardData->CardImage.LoadSynchronous();
        }
    }

    return nullptr;
}
