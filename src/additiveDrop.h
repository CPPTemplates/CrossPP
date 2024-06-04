#pragma once
#include "lootTable.h"
struct additiveDrop : public lootTable
{
	additiveDrop(const std::vector<lootTable*>& drops = std::vector<lootTable*>());
	std::vector<lootTable*> drops = std::vector<lootTable*>();
	virtual ~additiveDrop() override;
	virtual std::vector<itemStack> roll(const dropData& data, std::mt19937& randomToUse) const override;
};