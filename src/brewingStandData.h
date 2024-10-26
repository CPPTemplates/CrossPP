#pragma once
#include "blockData.h"
struct brewingstandData :blockData
{
	//the amount of fuel that is left from the last blaze powder. 1 fuel -> 1 batch
	int fuelLeft = 0;
	int ticksBrewed = 0;
	rectangularSlotContainer* blazePowderSlot = nullptr;
	rectangularSlotContainer* ingredientSlot = nullptr;
	rectangularSlotContainer* potionSlots[brewingStandPotionCapacity];

	brewingstandData();
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void onBlockRemove(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual ~brewingstandData() override;
	virtual void serializeValue(nbtSerializer& s) override;

};