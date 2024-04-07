// Fill out your copyright notice in the Description page of Project Settings.


#include "CardGameState.h"
#include <Net/UnrealNetwork.h>

ACardGameState::ACardGameState()
{
	bReplicates = true;
}

void ACardGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACardGameState, PlayerStateArray);
}

APlayerStateCustom* ACardGameState::GetPlayerStateByIndex(int32 index)
{
	//��������PlayerStateArray
	for (int32 i = 0; i < PlayerStateArray.Num(); i++)
	{
		//ת��PlayerStateArray[i]������Ϊ ACardPlatyerState
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			if(PlayerState->GetPlayerIndex() == index)
			{
				return PlayerState;
			}
		}
	}
	return nullptr;
}

void ACardGameState::AddPlayerStateArray(APlayerStateCustom* NewPlayerState)
{
	PlayerStateArray.Add(NewPlayerState);
}

void ACardGameState::RemovePlayerStateArray(APlayerStateCustom* PlayerState)
{
	PlayerStateArray.Remove(PlayerState);
}

int32 ACardGameState::GetPlayerStateReadyCount()
{
	int32 ReadyCount = 0;
	for (int32 i = 0; i < PlayerStateArray.Num(); i++)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			if (PlayerState->GetPlayerReady())
			{
				ReadyCount++;
			}
		}
	}
	return ReadyCount;
}

void ACardGameState::DealCardToPlayer(UDeck* CardDeck)
{
	//���ֻ���ڷ������˲���ִ��
	if (!HasAuthority())
	{
		return;
	}

	for (int32 i = 0; i < PlayerStateArray.Num(); i++)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			for (int32 j = 0; j < 10; ++j)
			{
				FCard Card = CardDeck->DealOneCard();
				PlayerState->AddCardToHand(Card);
			}
		}
	}
}

void ACardGameState::OnRep_PlayerStateArrayChange()
{
	for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
	{
		APlayerStateCustom* PlayerState = Cast<APlayerStateCustom>(PlayerStateArray[i]);
		if (PlayerState)
		{
			PlayerState->NotifyPlayerJoinMainMenuBase();
		}
	}

	//�����־,��ǰ��˭��CardGameState,�����ж��ٸ�PlayerState,�Լ�ÿ��PlayerState������
	UE_LOG(LogTemp, Warning, TEXT("CardGameState: %s, PlayerStateArray.Num(): %d"), *GetName(), PlayerStateArray.Num());

}
