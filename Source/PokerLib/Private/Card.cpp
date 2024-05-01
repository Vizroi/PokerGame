// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"

FString GetCardValueString(ECardValue Value)
{
	switch (Value)
	{
	case ECardValue::Ace:
		return "A";
	case ECardValue::J:
		return "J";
	case ECardValue::Q:
		return "Q";
	case ECardValue::K:
		return "K";
	case ECardValue::JokerA:
		return "JokerA";
	case ECardValue::JokerB:
		return "JokerB";
	default:
		return FString::FromInt(static_cast<int32>(Value));
	}
}

FString GetCardSuitString(ESuit Suit)
{
	//switch (Suit)
	//{
	//	case ESuit::Spade:
	//		return "����";
	//	case ESuit::Heart:
	//		return "����";
	//	case ESuit::Club:
	//		return "÷��";
	//	case ESuit::Diamond:
	//		return "��Ƭ";
	//	case ESuit::Joker:
	//		return "����";
	//}

	return UEnum::GetValueAsString(Suit);
}
