#pragma once
#include "slotContainer.h"
#include "interface/idestructable.h"
#include "rectangularSlotContainer.h"
struct uiSlotContainer :slotContainer, IDestructable
{
	veci2 containerPixelPosition;
	rectangularSlotContainer* linkedContainer;
	uiSlotContainer(veci2 containerPixelPosition, rectangularSlotContainer* linkedContainer = nullptr) :containerPixelPosition(containerPixelPosition), linkedContainer(linkedContainer) {}
	inline itemStack* getSlot(cveci2& pos) { return linkedContainer->getSlot(pos); }
	virtual bool addStack(itemStack& stack) override;
	//virtual void clickedOnItem(cmb& button, itemStack& stackHolding, veci2 selectedSlot);
	virtual ~uiSlotContainer() override;
};