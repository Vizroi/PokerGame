// Fill out your copyright notice in the Description page of Project Settings.


#include "Deck.h"

UDeck::UDeck()
{

}

void UDeck::InitializeDeck(EDeckMode Mode)
{
    switch (Mode)
    {
    case EDeckMode::Standard:
    {
        IDeckInitializationStrategy* Strategy = new StandardDeckInitializationStrategy();
        Strategy->InitializeDeck(Cards);
    }
        break;
    case EDeckMode::RedTen:
    {
		IDeckInitializationStrategy* Strategy = new RedTenDeckInitializationStrategy();
		Strategy->InitializeDeck(Cards);
    }
        break;
    case EDeckMode::TexasHoldem:
    {
        IDeckInitializationStrategy* Strategy = new TexasHoldemDeckInitializationStrategy();
		Strategy->InitializeDeck(Cards);
    }
        break;
    default:
    {
        IDeckInitializationStrategy* Strategy = new StandardDeckInitializationStrategy();
        Strategy->InitializeDeck(Cards);
    }
        break;
    }   
    ShuffleDeck();
}


void UDeck::ShuffleDeck()
{
    const int32 LastIndex = Cards.Num() - 1;
    for(int32  i = 0; i < LastIndex; ++i)
	{
		const int32 RandomIndex = FMath::RandRange(i, LastIndex);
		Cards.Swap(i, RandomIndex);
	}
}

FCard UDeck::DealOneCard()
{
    if (!Cards.IsEmpty())
    {
        return Cards.Pop(false);
    }
    return FCard();
}

bool UDeck::IsEmpty() const
{
    return Cards.IsEmpty();
}
