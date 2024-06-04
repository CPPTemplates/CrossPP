#pragma once
#include "craftableSlotContainer.h"
struct smithingTableSlotContainer : craftableSlotContainer
{
	uiSlotContainer* smithingInputSlot = nullptr;
	uiSlotContainer* netheriteIngotSlot = nullptr;
	smithingTableSlotContainer();
	virtual bool addStack(itemStack& stack) override;
	virtual ~smithingTableSlotContainer() override;
	virtual itemStack calculateRecipeResult() override;
	bool canAddStack(uiSlotContainer* containerToAddTo, stackDivider& s) override;
	void substractCraftingIngredients() override;
};