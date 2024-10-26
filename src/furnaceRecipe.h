#pragma once
#include "recipe.h"
#include "IItemComparable.h"
struct furnaceRecipe : recipe
{
	IItemComparable* requiredItem = nullptr;
	fp experience = 0;//the average amount of experience you will get when smelting
	int cookingTime = 0;//the amount of ticks it costs to cook one item
	virtual bool verifyPattern(rectangularSlotContainer* container) override;
};
extern std::vector<furnaceRecipe*> furnaceRecipes;

itemID getCookedItem(const itemID& identifier);