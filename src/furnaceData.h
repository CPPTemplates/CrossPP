#pragma once
#include "blockData.h"
#include "furnaceRecipe.h"
struct furnaceData :blockData
{
	furnaceData();
	rectangularSlotContainer* inputSlot = nullptr;
	rectangularSlotContainer* outputSlot = nullptr;
	rectangularSlotContainer* fuelSlot = nullptr;
	int ticksCooked = 0;
	int ticksFuelBurned = 0;
	int ticksFuelToBurn = 0;
	fp collectedExperience = 0;
	furnaceRecipe* currentRecipe = nullptr;
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void onBlockRemove(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual ~furnaceData() override;
	virtual void serializeValue(nbtSerializer& s) override;
};