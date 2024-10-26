#pragma once
#include "lootTable.h"
struct explosionDecayDrop : public lootTable
{
	lootTable* dropsToDecay = nullptr;
	explosionDecayDrop(lootTable* dropsToDecay = nullptr) :dropsToDecay(dropsToDecay) {}
	virtual std::vector<itemStack> roll(const dropData& data, std::mt19937& randomToUse) const override;
	virtual ~explosionDecayDrop() override;
};