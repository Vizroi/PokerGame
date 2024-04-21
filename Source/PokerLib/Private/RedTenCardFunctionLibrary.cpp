// Fill out your copyright notice in the Description page of Project Settings.


#include "RedTenCardFunctionLibrary.h"


FCardSet URedTenCardFunctionLibrary::DetermineCardSetType(const TArray<FCard>& Cards)
{
	FCardSet Result;
	Result.Cards = Cards;
	Result.HighestCard = FCard();

	//根据卡牌数量来决定卡牌类型
	if (IsTwoPairOfRedTen(Cards))
	{
		Result.Type = ECardSetType::RedTens;
		Result.HighestCard = GetCardHighestCard(Cards);
	}
	else if (IsBigBomb(Cards))
	{
		Result.Type = ECardSetType::BigBomb;
		Result.HighestCard = GetCardHighestCard(Cards);
	}
	else if (IsBomb(Cards))
	{
		Result.Type = ECardSetType::Bomb;
		Result.HighestCard = GetCardHighestCard(Cards);
	}
	else if (IsPair(Cards))
	{
		Result.Type = ECardSetType::Pair;
		Result.HighestCard = GetCardHighestCard(Cards);
	}
	else if (IsStraight(Cards))
	{
		Result.Type = ECardSetType::Straight;
		Result.HighestCard = GetCardHighestCard(Cards);
	}
	else if (IsSingle(Cards))
	{
		Result.Type = ECardSetType::Single;
		Result.HighestCard = GetCardHighestCard(Cards);
	}
	else if (Cards.Num() == 0)
	{
		Result.Type = ECardSetType::Empty;
		Result.HighestCard = FCard();
	}
	else
	{
		Result.Type = ECardSetType::Invalid;
	}

	return Result;
}

bool URedTenCardFunctionLibrary::CompareCards(const TArray<FCard>& CurrentCards, const TArray<FCard>& LastCards)
{
	FCardSet CurrentCardSet = DetermineCardSetType(CurrentCards);
	FCardSet LastCardsSet = DetermineCardSetType(LastCards);

	if (CurrentCardSet.Type == ECardSetType::Invalid || CurrentCardSet.Type == ECardSetType::Empty)
	{
		return false;
	}

	if (LastCardsSet.Type == ECardSetType::Empty)
	{
		return true;
	}

	if (LastCardsSet.Type == ECardSetType::RedTens)
	{
		return false;
	}

	if (CurrentCardSet.Type == ECardSetType::RedTens)
	{
		return true;
	}

	if (LastCardsSet.Type == ECardSetType::BigBomb)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb)
		{
			if (CompareCardsPriority(CurrentCardSet.HighestCard, LastCardsSet.HighestCard) || CurrentCards.Num() > LastCards.Num())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if (LastCardsSet.Type == ECardSetType::Bomb)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb)
		{
			return true;
		}
		else if (CurrentCardSet.Type == ECardSetType::Bomb)
		{
			return CompareCardsPriority(CurrentCardSet.HighestCard, LastCardsSet.HighestCard);
		}
		else
		{
			return false;
		}
	}

	if (LastCardsSet.Type == ECardSetType::Pair)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb || CurrentCardSet.Type == ECardSetType::Bomb)
		{
			return true;
		}
		else if (CurrentCardSet.Type == ECardSetType::Pair)
		{
			return CompareCardsPriority(CurrentCardSet.HighestCard, LastCardsSet.HighestCard);
		}
		else
		{
			return false;
		}
	}

	if (LastCardsSet.Type == ECardSetType::Straight)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb || CurrentCardSet.Type == ECardSetType::Bomb)
		{
			return true;
		}
		else if (CurrentCardSet.Type == ECardSetType::Straight && CurrentCards.Num() == LastCards.Num())
		{
			return CompareCardsPriority(CurrentCardSet.HighestCard, LastCardsSet.HighestCard);
		}
		else
		{
			return false;
		}
	}

	if (LastCardsSet.Type == ECardSetType::Single)
	{
		if (CurrentCardSet.Type == ECardSetType::BigBomb || CurrentCardSet.Type == ECardSetType::Bomb)
		{
			return true;
		}
		else if (CurrentCardSet.Type == ECardSetType::Single)
		{
			return CompareCardsPriority(CurrentCardSet.HighestCard, LastCardsSet.HighestCard);
		}
		else
		{
			return false;
		}
	}

	return false;
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

bool URedTenCardFunctionLibrary::IsRedTen(const FCard& Card)
{
	if(Card.Suit == ESuit::Heart && Card.Value == ECardValue::Ten)
	{
		return true;
	}
	else if (Card.Suit == ESuit::Diamond && Card.Value == ECardValue::Ten)
	{
		return true;
	}
	return false;
}

bool URedTenCardFunctionLibrary::IsCardsUniformSet(const TArray<FCard>& Cards, int32 RequiredBombCount)
{
	if (Cards.Num() < RequiredBombCount)
	{
		return false;
	}

	FCard ReferenceCard;
	for (int32 i = 0; i < Cards.Num(); ++i)
	{
		if (ReferenceCard.IsEmpty() && !IsJoker(Cards[i]))
		{
			ReferenceCard = Cards[i];
		}
		else
		{
			if ((ReferenceCard.Value == Cards[i].Value && !IsRedTen(Cards[i])) 
											|| IsJoker(Cards[i]))
			{
				continue;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

bool URedTenCardFunctionLibrary::IsBigBomb(const TArray<FCard>& Cards)
{
	return IsCardsUniformSet(Cards, 4);
}

bool URedTenCardFunctionLibrary::IsBomb(const TArray<FCard>& Cards)
{
	return IsCardsUniformSet(Cards, 3);
}

bool URedTenCardFunctionLibrary::IsPair(const TArray<FCard>& Cards)
{
	return IsCardsUniformSet(Cards, 2);
}

bool URedTenCardFunctionLibrary::IsStraight(const TArray<FCard>& Cards)
{
	if (Cards.Num() < 3)
	{
		return false;
	}

	for (auto Elem : Cards)
	{
		if (IsRedTen(Elem))
		{
			return false;
		}
	}

	TArray<FCard> SortedValues = Cards;
	SortCardsForStraightPriority(SortedValues);

	int32 JokerCount = 0;
	int32 LastValue = -1;
	int32 Gaps = 0;

	for (const FCard& Card : SortedValues)
	{
		if (IsJoker(Card))
		{
			JokerCount++;
			continue;
		}

		int CurrentValue = GetCardStraightPriority(Card);
		if (LastValue != -1)
		{
			int Gap = CurrentValue - LastValue - 1;
			if (Gap > 0)
			{
				Gaps += Gap;
			}
		}

		LastValue = CurrentValue;
	}

	return JokerCount >= Gaps;
}

bool URedTenCardFunctionLibrary::IsSingle(const TArray<FCard>& Cards)
{
	if (Cards.Num() != 1)
	{
		return false;
	}

	return true;
}

FCard URedTenCardFunctionLibrary::GetCardHighestCard(const TArray<FCard>& Cards)
{
	if (Cards.IsEmpty())
	{
		return FCard();
	}

	FCard HighestCard = Cards[0];
	for(const FCard& Card : Cards)
	{
		if (GetCardPriority(Card) < GetCardPriority(HighestCard))
		{
			HighestCard = Card;
		}
	}

	return HighestCard;
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

	return 99;
}

int32 URedTenCardFunctionLibrary::GetCardStraightPriority(const FCard& Card)
{
	if (Card.Value == ECardValue::Six)
		return 1;
	if (Card.Value == ECardValue::Seven)
		return 2;
	if (Card.Value == ECardValue::Eight)
		return 3;
	if (Card.Value == ECardValue::Nine)
		return 4;
	if (Card.Value == ECardValue::Ten && (Card.Suit != ESuit::Heart || Card.Suit != ESuit::Diamond))
		return 5;
	if (Card.Value == ECardValue::J)
		return 6;
	if (Card.Value == ECardValue::Q)
		return 7;
	if (Card.Value == ECardValue::K)
		return 8;
	if (Card.Value == ECardValue::Ace)
		return 9;

	return -1;
}

bool URedTenCardFunctionLibrary::CompareCardsPriority(const FCard& Card1, const FCard& Card2)
{
	if (GetCardPriority(Card1) < GetCardPriority(Card2))
	{
		return true;
	}
	/*
	else if (GetCardPriority(Card1) == GetCardPriority(Card2))
	{
		return static_cast<int32>(Card1.Suit) > static_cast<int32>(Card2.Suit);
	}
	*/
	return false;
}

int32 URedTenCardFunctionLibrary::SortCardsForPriority(TArray<FCard>& Cards)
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

int32 URedTenCardFunctionLibrary::SortCardsForStraightPriority(TArray<FCard>& Cards)
{
	if (Cards.Num() > 1)
	{
		Cards.Sort([](const FCard& A, const FCard& B)
			{
				int32 PriorityA = GetCardStraightPriority(A);
				int32 PriorityB = GetCardStraightPriority(B);
				if (PriorityA != PriorityB)
				{
					return PriorityA < PriorityB;
				}

				return static_cast<int32>(A.Suit) < static_cast<int32>(B.Suit);
			});
	}
	return int32();
}

