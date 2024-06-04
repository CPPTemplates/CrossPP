#include "craftableSlotContainer.h"
#include "recipe.h"
#include "human.h"
#include "dimension.h"
#include "stackDivider.h"
void craftableSlotContainer::substractCraftingIngredients()
{
	for (uiSlotContainer* container : craftingInputContainers)
	{
		substractOneOfEachSlot(container->linkedContainer);
	}
}

void craftableSlotContainer::recalculateOutputSlot()
{
	human* currentHuman = (human*)linkedPlayer;
	itemStack newRecipeResult = calculateRecipeResult();
	if (currentRecipeResult.count && (!currentRecipeResult.compare(newRecipeResult)))
	{
		//the player still has some items left from the last crafting session
		int outputCount = craftingOutputSlot->linkedContainer->slots[0].count;
		if (outputCount < currentRecipeResult.count)
		{
			craftingOutputSlot->linkedContainer->transfer((human*)linkedPlayer, linkedPlayer->dimensionIn, currentHuman->getDropPosition());
		}
	}
	currentRecipeResult.clearData();
	craftingOutputSlot->linkedContainer->clearData();

	currentRecipeResult = newRecipeResult;

	//update result slot
	craftingOutputSlot->linkedContainer->slots[0] = currentRecipeResult.count ? currentRecipeResult : itemStack();
}

void craftableSlotContainer::clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot)
{
	if (selectedSlotContainer == craftingOutputSlot)
	{
		if (currentRecipeResult.count && divider.divideOver.size() == 0)
		{
			itemStack* s = selectedSlotContainer->getSlot(selectedSlot);
			cint previousOutputSlotCount = s->count;
			if (divider.stackHolding.count == 0 || s->compare(divider.stackHolding))
			{
				//add crafting result
                divider.addStack(*s);

				if (previousOutputSlotCount == currentRecipeResult.count && s->count < currentRecipeResult.count)
				{
					substractCraftingIngredients();
				}

				if (s->count == 0) //there are no items left in the crafting output slot
				{
					recalculateOutputSlot();
				}
			}
		}
	}
	else
	{
		inventory::clickedOnItem(button, divider, selectedSlotContainer, selectedSlot);
        //when dragging items across different slot containers, the result of a the recipe may also change. but
		if (std::find(craftingInputContainers.begin(), craftingInputContainers.end(), selectedSlotContainer) != craftingInputContainers.end())
		{
			recalculateOutputSlot();
		}
	}
}