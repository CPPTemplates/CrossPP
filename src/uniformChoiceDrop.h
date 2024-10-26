#pragma once
#include "lootTable.h"
struct uniformChoiceDrop : public lootTable
{
	std::vector<lootTable*> options;
	uniformChoiceDrop(const std::vector<lootTable*>& options = std::vector<lootTable*>()) :options(options) {}
	inline std::vector<itemStack> roll(const dropData& data, std::mt19937& randomToUse) const override;
	virtual ~uniformChoiceDrop() override;
};