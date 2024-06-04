#include "craftingGridSlotContainer.h"
#include "recipe.h"
itemStack craftingGridSlotContainer::calculateRecipeResult()
{
	//recalculate crafting recipe
	recipe* newRecipe = findRecipe(craftingRecipes, craftingSlots->linkedContainer);//getCraftingRecipe(craftingSlots->linkedContainer);

	return newRecipe ? newRecipe->result : itemStack();
}
craftingGridSlotContainer::craftingGridSlotContainer(uiSlotContainer* craftingSlots)
{
	this->craftingSlots = craftingSlots;
	craftingInputContainers = { craftingSlots };
}
