#pragma once
#include "lootTable.h"
#include "amountDistribution.h"
struct amountDrop : public lootTable
{
	lootTable* dropsToIncrease = nullptr;
	amountDistribution* distribution = nullptr;
	amountDrop(lootTable* dropsToIncrease, amountDistribution* distribution) :dropsToIncrease(dropsToIncrease), distribution(distribution) {}
	virtual std::vector<itemStack> roll(const dropData& data, std::mt19937& randomToUse) const override;
	virtual ~amountDrop() override;
};