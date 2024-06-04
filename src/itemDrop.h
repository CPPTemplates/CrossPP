#pragma once
#include "lootTable.h"
struct itemDrop : public lootTable
{
	itemID itemType = (itemID)0;
	itemDrop(const itemID& itemType) :itemType(itemType) {}
	virtual std::vector<itemStack> roll(const dropData& data, std::mt19937& randomToUse) const override;
};