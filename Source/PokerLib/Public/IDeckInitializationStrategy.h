// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Card.h"

/**
 * 卡组初始化策略接口
 */
class POKERLIB_API IDeckInitializationStrategy
{
public:
    virtual ~IDeckInitializationStrategy() = default;

    // 根据策略初始化卡组
    virtual void InitializeDeck(TArray<FCard>& Cards) = 0;
};

class StandardDeckInitializationStrategy : public IDeckInitializationStrategy
{
public:
    virtual void InitializeDeck(TArray<FCard>& Cards) override;
};

class RedTenDeckInitializationStrategy : public IDeckInitializationStrategy
{
public:
	virtual void InitializeDeck(TArray<FCard>& Cards) override;
};

class TexasHoldemDeckInitializationStrategy : public IDeckInitializationStrategy
{
public:
	virtual void InitializeDeck(TArray<FCard>& Cards) override;
};
