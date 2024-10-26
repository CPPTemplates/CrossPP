#pragma once
#include "craftingGridSlotContainer.h"
struct craftingTableSlotContainer : craftingGridSlotContainer
{
	craftingTableSlotContainer();
	virtual bool addStack(itemStack& stack) override;
	virtual ~craftingTableSlotContainer() override;
};