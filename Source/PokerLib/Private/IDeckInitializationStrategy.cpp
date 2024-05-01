// Fill out your copyright notice in the Description page of Project Settings.


#include "IDeckInitializationStrategy.h"

void StandardDeckInitializationStrategy::InitializeDeck(TArray<FCard>& Cards)
{
    Cards.Empty();

    int32 CardID = 0;
    for (int32 SuitInt = 0; SuitInt < static_cast<int32>(ESuit::Joker); ++SuitInt)
    {
        for (int32 Value = static_cast<int32>(ECardValue::Ace); Value <= static_cast<int32>(ECardValue::K); ++Value)
        {
            Cards.Add(FCard(CardID,static_cast<ESuit>(SuitInt), static_cast<ECardValue>(Value), false));
            ++CardID;
        }
    }

    // 添加两张王
    Cards.Add(FCard(CardID, ESuit::Joker, ECardValue::JokerA, false));
    Cards.Add(FCard(CardID + 1, ESuit::Joker, ECardValue::JokerB, false));
}

void RedTenDeckInitializationStrategy::InitializeDeck(TArray<FCard>& Cards)
{
    Cards.Empty(); // 清空现有卡组

    int32 CardID = 0;
    // 添加四种花色的A到K
    for (int32 Suit = 0; Suit < 4; ++Suit)
    {
        for (int32 Value = 1; Value <= 13; ++Value)
        {
            // 如果是红十模式，跳过3,4,5和指定的梅花6、梅花7
            if (Value == 3 || Value == 4 || Value == 5 ||
                (Suit == static_cast<int32>(ESuit::Club) && (Value == 6 || Value == 7)))
            {
                continue;
            }

            Cards.Add(FCard(CardID, static_cast<ESuit>(Suit), static_cast<ECardValue>(Value), false));
            ++CardID;
        }
    }

    // 添加大小王
    Cards.Add(FCard(CardID, ESuit::Joker, ECardValue::JokerA, false));
    Cards.Add(FCard(CardID + 1, ESuit::Joker, ECardValue::JokerB, false));
}

void TexasHoldemDeckInitializationStrategy::InitializeDeck(TArray<FCard>& Cards)
{
    Cards.Empty();

    int32 CardID = 0;
    for (int32 SuitInt = 0; SuitInt < static_cast<int32>(ESuit::Joker); ++SuitInt)
    {
        for (int32 Value = static_cast<int32>(ECardValue::Ace); Value <= static_cast<int32>(ECardValue::K); ++Value)
        {
            Cards.Add(FCard(CardID, static_cast<ESuit>(SuitInt), static_cast<ECardValue>(Value), false));
            ++CardID;
        }
    }
}
