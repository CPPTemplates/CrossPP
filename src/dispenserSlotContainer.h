#pragma once
#include "inventory.h"
#include "dispenserData.h"
struct dispenserSlotContainer : inventory
{
	dispenserData* selectedDispenserData = nullptr;
	uiSlotContainer* dispenserSlots = nullptr;
	dispenserSlotContainer();
	virtual bool addStack(itemStack& stack) override;
	virtual ~dispenserSlotContainer() override;
};