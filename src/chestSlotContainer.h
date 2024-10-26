#pragma once
#include "inventory.h"
struct chestSlotContainer :inventory
{
	chestData* selectedChestData = nullptr;
	uiSlotContainer* chestSlots = nullptr;
	chestSlotContainer();
	virtual bool addStack(itemStack& stack) override;
	virtual ~chestSlotContainer() override;
};