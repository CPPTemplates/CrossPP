#pragma once
#include "blockData.h"
struct chestData :blockData
{
	chestData();
	std::wstring lootTableName = std::wstring();
	int64_t lootTableSeed = 0;
	rectangularSlotContainer* slots = nullptr;
	virtual void onBlockRemove(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual ~chestData() override;
	virtual void serializeValue(nbtSerializer& s) override;
	void generateChestLoot();
};