#pragma once
#include "lootTable.h"
struct choiceDrop : public lootTable
{
	std::vector<lootTable*> options;
	std::vector<fp> chances;
	inline choiceDrop(const std::vector<lootTable*>& options = std::vector<lootTable*>(), const std::vector<fp>& chances = std::vector<fp>()) :options(options), chances(chances) {}
	inline std::vector<itemStack> roll(const dropData& data, std::mt19937& randomToUse) const override;
	virtual ~choiceDrop() override;
};