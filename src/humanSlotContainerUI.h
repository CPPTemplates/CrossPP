#pragma once
#include "craftingGridSlotContainer.h"
struct humanSlotContainerUI :craftingGridSlotContainer
{
	uiSlotContainer* armorSlots;
	uiSlotContainer* leftHandSlot;
	humanSlotContainerUI();
	virtual void clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot) override;
};