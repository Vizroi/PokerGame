// Fill out your copyright notice in the Description page of Project Settings.


#include "IDeckInitializationStrategy.h"

void StandardDeckInitializationStrategy::InitializeDeck(TArray<FCard>& Cards)
{
    Cards.Empty();

    for (int32 SuitInt = 0; SuitInt < static_cast<int32>(ESuit::Joker); ++SuitInt)
    {
        for (int32 Value = static_cast<int32>(ECardValue::Ace); Value <= static_cast<int32>(ECardValue::K); ++Value)
        {
            Cards.Add(FCard(static_cast<ESuit>(SuitInt), static_cast<ECardValue>(Value)));
        }
    }

    // 添加两张王
    Cards.Add(FCard(ESuit::Joker, ECardValue::JokerA));
    Cards.Add(FCard(ESuit::Joker, ECardValue::JokerB));
}

void RedTenDeckInitializationStrategy::InitializeDeck(TArray<FCard>& Cards)
{
    Cards.Empty(); // 清空现有卡组

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

            Cards.Add(FCard(static_cast<ESuit>(Suit), static_cast<ECardValue>(Value)));
        }
    }

    // 添加大小王
    Cards.Add(FCard(ESuit::Joker, ECardValue::JokerA));
    Cards.Add(FCard(ESuit::Joker, ECardValue::JokerB));
}

void TexasHoldemDeckInitializationStrategy::InitializeDeck(TArray<FCard>& Cards)
{
    Cards.Empty();

    for (int32 SuitInt = 0; SuitInt < static_cast<int32>(ESuit::Joker); ++SuitInt)
    {
        for (int32 Value = static_cast<int32>(ECardValue::Ace); Value <= static_cast<int32>(ECardValue::K); ++Value)
        {
            Cards.Add(FCard(static_cast<ESuit>(SuitInt), static_cast<ECardValue>(Value)));
        }
    }
}
