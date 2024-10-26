#pragma once
#include "inventory.h"
struct furnaceSlotContainer : inventory
{
	furnaceData* selectedFurnaceData = nullptr;
	uiSlotContainer* furnaceOutputSlot = nullptr;
	uiSlotContainer* furnaceInputSlot = nullptr;
	uiSlotContainer* furnaceFuelSlot = nullptr;
	furnaceSlotContainer();
	virtual bool addStack(itemStack& stack) override;
	virtual void drawExtraData(cmat3x3& transform, const texture& renderTarget) override;
	virtual ~furnaceSlotContainer() override;
	virtual void clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot) override;
};