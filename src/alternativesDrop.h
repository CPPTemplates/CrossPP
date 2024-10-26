#pragma once
#include "lootTable.h"
struct alternativesDrop : public lootTable
{
	std::vector<lootTable*> alternatives;
	alternativesDrop(const std::vector<lootTable*>& alternatives = std::vector<lootTable*>()) :alternatives(alternatives) {}
	inline std::vector<itemStack> roll(const dropData& data, std::mt19937& randomToUse) const override;
	virtual ~alternativesDrop() override;
};