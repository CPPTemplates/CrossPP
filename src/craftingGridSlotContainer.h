#pragma once
#include "craftableSlotContainer.h"
struct craftingGridSlotContainer : craftableSlotContainer
{
	uiSlotContainer* craftingSlots = nullptr;
	virtual itemStack calculateRecipeResult() override;
	craftingGridSlotContainer(uiSlotContainer* craftingSlots);
};