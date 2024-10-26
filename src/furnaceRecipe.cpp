#include "furnaceRecipe.h"

std::vector<furnaceRecipe*> furnaceRecipes = std::vector<furnaceRecipe*>();

itemID getCookedItem(const itemID& identifier)
{
	//find recipe
	rectangularSlotContainer container = rectangularSlotContainer(cveci2(1));
	container.slots[0].stackItemID = identifier;
	recipe* r = findRecipe(furnaceRecipes, &container);

	if (r)
	{
		return r->result.stackItemID;
	}
	else
	{
		return identifier;
	}
	return itemID();
}
bool furnaceRecipe::verifyPattern(rectangularSlotContainer* container)
{
	return requiredItem->compare(container->slots[0].stackItemID);
}