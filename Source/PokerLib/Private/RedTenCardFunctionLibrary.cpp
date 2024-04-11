// Fill out your copyright notice in the Description page of Project Settings.


#include "RedTenCardFunctionLibrary.h"


FCardSet URedTenCardFunctionLibrary::DetermineCardSetType(const TArray<FCard>& Cards)
{
	FCardSet Result;
	Result.Cards = Cards;

	//根据卡牌数量来决定卡牌类型
	if (IsTwoPairOfRedTen(Cards))
	{
		Result.Type = ECardSetType::RedTens;
	}
	else if (IsBigBomb(Cards, Result.HighestValue))
	{
		Result.Type = ECardSetType::BigBomb;
	}
	else if (IsBomb(Cards, Result.HighestValue))
	{
		Result.Type = ECardSetType::Bomb;
	}
	else if (IsPair(Cards, Result.HighestValue))
	{
		Result.Type = ECardSetType::Pair;
	}
	else if (IsStraight(Cards, Result.HighestValue))
	{
		Result.Type = ECardSetType::Straight;
	}
	else if (IsSingle(Cards, Result.HighestValue))
	{
		Result.Type = ECardSetType::Single;
	}
	else if (Cards.Num() == 0)
	{
		Result.Type = ECardSetType::Empty;
	}
	else
	{
		Result.Type = ECardSetType::Invalid;
	}

	return Result;
}

bool URedTenCardFunctionLibrary::CanPlayCard(const TArray<FCard>& CurrentCards, const TArray<FCard>& LastCards)
{
	FCardSet CurrentCardSet = DetermineCardSetType(CurrentCards);
	FCardSet PlayerCardSet = DetermineCardSetType(LastCards);

	if (CurrentCardSet.Type == ECardSetType::Invalid || CurrentCardSet.Type == ECardSetType::Empty)
	{
		return false;
	}

	if (PlayerCardSet.Type == ECardSetType::RedTens)
	{
		return false;
	}

	if (PlayerCardSet.Type == ECardSetType::BigBomb)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb)
		{
			return CurrentCardSet.HighestValue > PlayerCardSet.HighestValue;
		}
		else
		{
			return false;
		}
	}

	if (PlayerCardSet.Type == ECardSetType::Bomb)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb)
		{
			return true;
		}
		else if (CurrentCardSet.Type == ECardSetType::Bomb)
		{
			return CurrentCardSet.HighestValue > PlayerCardSet.HighestValue;
		}
		else
		{
			return false;
		}
	}

	if (PlayerCardSet.Type == ECardSetType::Pair)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb || CurrentCardSet.Type == ECardSetType::Bomb)
		{
			return true;
		}
		else if (CurrentCardSet.Type == ECardSetType::Pair)
		{
			return CurrentCardSet.HighestValue > PlayerCardSet.HighestValue;
		}
		else
		{
			return false;
		}
	}

	if (PlayerCardSet.Type == ECardSetType::Straight)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb || CurrentCardSet.Type == ECardSetType::Bomb)
		{
			return true;
		}
		else if (CurrentCardSet.Type == ECardSetType::Straight)
		{
			return CurrentCardSet.HighestValue > PlayerCardSet.HighestValue;
		}
		else
		{
			return false;
		}
	}

	if (PlayerCardSet.Type == ECardSetType::Single)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb || CurrentCardSet.Type == ECardSetType::Bomb)
		{
			return true;
		}
		else if (CurrentCardSet.Type == ECardSetType::Single)
		{
			return CurrentCardSet.HighestValue > PlayerCardSet.HighestValue;
		}
		else
		{
			return false;
		}
	}

	if (PlayerCardSet.Type == ECardSetType::Empty)
	{
		return true;
	}


	return false;
}

void URedTenCardFunctionLibrary::CountCards(const TArray<FCard>& Cards, TMap<ECardValue, int32>& CardCounts, int32& JokerCount)
{
	CardCounts.Empty();
	JokerCount = 0;

	for (const FCard& Card : Cards)
	{
		if (IsJoker(Card))
		{
			++JokerCount;
		}
		else
		{
			CardCounts.FindOrAdd(Card.Value)++;
		}
	}
}

bool URedTenCardFunctionLibrary::IsTwoPairOfRedTen(const TArray<FCard>& Cards)
{
	if (Cards.Num() == 2)
	{
		if (Cards[0].Value == ECardValue::Ten && Cards[1].Value == ECardValue::Ten)
		{
			if (Cards[0].Suit == ESuit::Heart && Cards[1].Suit == ESuit::Diamond)
			{
				return true;
			}
		}
	}

	return false;
}

bool URedTenCardFunctionLibrary::IsBigBomb(const TArray<FCard>& Cards, ECardValue& HightestValue)
{
	TMap<ECardValue, int32> CardCounts;
	int32 JokerCount = 0;
	CountCards(Cards, CardCounts, JokerCount);

	for (const TPair<ECardValue, int32>& Pair : CardCounts)
	{
		if (Pair.Value + JokerCount >= 4)
		{
			HightestValue = Pair.Key;
			return true;
		}
	}

	return false;
}

bool URedTenCardFunctionLibrary::IsBomb(const TArray<FCard>& Cards, ECardValue& HightestValue)
{
	if (Cards.Num() != 3)
	{
		return false;
	}

	TMap<ECardValue, int32> CardCounts;
	int32 JokerCount = 0;
	CountCards(Cards, CardCounts, JokerCount);

	for (const TPair<ECardValue, int32>& Pair : CardCounts)
	{
		if (Pair.Value + JokerCount == 3)
		{
			HightestValue = Pair.Key;
			return true;
		}
	}

	return false;
}

bool URedTenCardFunctionLibrary::IsPair(const TArray<FCard>& Cards, ECardValue& HightestValue)
{
	if (Cards.Num() != 2)
	{
		return false;
	}

	TMap<ECardValue, int32> CardCounts;
	int32 JokerCount = 0;
	CountCards(Cards, CardCounts, JokerCount);

	for (const TPair<ECardValue, int32>& Pair : CardCounts)
	{
		if (Pair.Value + JokerCount == 2)
		{
			HightestValue = Pair.Key;
			return true;
		}
	}

	return false;
}

bool URedTenCardFunctionLibrary::IsStraight(const TArray<FCard>& Cards, ECardValue& HightestValue)
{
	if (Cards.Num() < 3)
	{
		return false;
	}
	else
	{
		return true;
	}

	// 用于计数每个牌值的数量，癞子另外计数
	TMap<ECardValue, int32> CardCounts;
	int32 JokerCount = 0;

	for (const FCard& Card : Cards)
	{
		if (IsJoker(Card))
		{
			JokerCount++;
		}
		else
		{
			// 排除红桃10和方片10，以及2
			if (Card.Value != ECardValue::Ten || Card.Value != ECardValue::Two)
			{
				CardCounts.FindOrAdd(Card.Value)++;
			}
		}
	}

	// 将牌值排序，以检查连续性
	TArray<ECardValue> SortedValues;
	CardCounts.GetKeys(SortedValues);
	SortedValues.Sort();
	//SortedValues.Sort([](const FCard& A, const FCard& B)
	//	{
	//		// 鬼牌作为最小
	//		if (A.Suit == ESuit::Joker) return true;
	//		if (B.Suit == ESuit::Joker) return false;

	//		// 对10进行特殊处理
	//		if (A.Value == ECardValue::Ten && (A.Suit == ESuit::Heart || A.Suit == ESuit::Diamond)) return false;
	//		if (B.Value == ECardValue::Ten && (B.Suit == ESuit::Heart || B.Suit == ESuit::Diamond)) return true;

	//		// 正常牌值排序，2和10（红桃、方片）作为最大
	//		int32 AValue = (A.Value == ECardValue::Two) ? static_cast<int32>(ECardValue::Ace) + 2 : static_cast<int32>(A.Value);
	//		int32 BValue = (B.Value == ECardValue::Two) ? static_cast<int32>(ECardValue::Ace) + 2 : static_cast<int32>(B.Value);

	//		// 正常排序，但将2和特殊的10置于末尾
	//		if (A.Value != ECardValue::Ten && B.Value == ECardValue::Ten && (B.Suit == ESuit::Heart || B.Suit == ESuit::Diamond))
	//		{
	//			return AValue < BValue + 1;
	//		}
	//		else if (A.Value == ECardValue::Ten && (A.Suit == ESuit::Heart || A.Suit == ESuit::Diamond) && B.Value != ECardValue::Ten)
	//		{
	//			return AValue + 1 < BValue;
	//		}

	//		return AValue < BValue;
	//	});

	int32 MissingCards = 0;
	int32 LastValue = static_cast<int32>(SortedValues[0]);

	// 特殊处理，允许QKA顺子
	if (SortedValues.Contains(ECardValue::Q) &&
		SortedValues.Contains(ECardValue::K) &&
		SortedValues.Contains(ECardValue::Ace))
	{
		return true;
	}

	for (int32 i = 1; i < SortedValues.Num(); i++)
	{
		// 计算缺失的牌数来判断是否能用癞子填补形成顺子
		int CurrentValue = static_cast<int32>(SortedValues[i]);
		int Gap = CurrentValue - LastValue - 1;
		MissingCards += FMath::Max(0, Gap);

		LastValue = CurrentValue;

		// 如果缺失的牌数已经超过了癞子的数量，则不能形成顺子
		if (MissingCards > JokerCount)
		{
			return false;
		}
	}

	// TODO: 没有返回顺子中的最高值

	return true;
}

bool URedTenCardFunctionLibrary::IsSingle(const TArray<FCard>& Cards, ECardValue& HightestValue)
{
	if (Cards.Num() != 1)
	{
		return false;
	}

	HightestValue = Cards[0].Value;

	return true;
}

int32 URedTenCardFunctionLibrary::GetCardPriority(const FCard& Card)
{
	if(Card.Value == ECardValue::Ten && (Card.Suit == ESuit::Heart || Card.Suit == ESuit::Diamond))
		return 1;
	if (Card.Value == ECardValue::Two)
		return 2;
	if (Card.Value == ECardValue::Ace)
		return 3;
	if (Card.Value == ECardValue::K)
		return 4;
	if (Card.Value == ECardValue::Q)
		return 5;
	if (Card.Value == ECardValue::J)
		return 6;
	if (Card.Value == ECardValue::Ten)
		return 7;
	if (Card.Value == ECardValue::Nine)
		return 8;
	if (Card.Value == ECardValue::Eight)
		return 9;
	if (Card.Value == ECardValue::Seven)
		return 10;
	if (Card.Value == ECardValue::Six)
		return 11;
	if (Card.Value == ECardValue::JokerA)
		return 12;
	if (Card.Value == ECardValue::JokerB)
		return 13;

	return 14; // 默认优先级，用于处理未知的牌
}

int32 URedTenCardFunctionLibrary::SortCards(TArray<FCard>& Cards)
{
	if (Cards.Num() > 1)
	{
		Cards.Sort([](const FCard& A, const FCard& B)
			{
				int32 PriorityA = GetCardPriority(A);
				int32 PriorityB = GetCardPriority(B);
				if (PriorityA != PriorityB)
				{
					return PriorityA < PriorityB;
				}

				return static_cast<int32>(A.Suit) < static_cast<int32>(B.Suit);
			});
	}
	return int32();
}

bool URedTenCardFunctionLibrary::CustomRedTenCardSort(const FCard& A, const FCard& B)
{
	// 鬼牌作为最小
	if (A.Suit == ESuit::Joker) return true;
	if (B.Suit == ESuit::Joker) return false;

	// 对10进行特殊处理
	if (A.Value == ECardValue::Ten && (A.Suit == ESuit::Heart || A.Suit == ESuit::Diamond)) return false;
	if (B.Value == ECardValue::Ten && (B.Suit == ESuit::Heart || B.Suit == ESuit::Diamond)) return true;

	// 正常牌值排序，2和10（红桃、方片）作为最大
	int32 AValue = (A.Value == ECardValue::Two) ? static_cast<int32>(ECardValue::Ace) + 2 : static_cast<int32>(A.Value);
	int32 BValue = (B.Value == ECardValue::Two) ? static_cast<int32>(ECardValue::Ace) + 2 : static_cast<int32>(B.Value);

	// 正常排序，但将2和特殊的10置于末尾
	if (A.Value != ECardValue::Ten && B.Value == ECardValue::Ten && (B.Suit == ESuit::Heart || B.Suit == ESuit::Diamond))
	{
		return AValue < BValue + 1;
	}
	else if (A.Value == ECardValue::Ten && (A.Suit == ESuit::Heart || A.Suit == ESuit::Diamond) && B.Value != ECardValue::Ten)
	{
		return AValue + 1 < BValue;
	}

	return AValue < BValue;
}
