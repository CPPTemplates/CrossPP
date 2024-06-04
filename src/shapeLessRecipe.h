#pragma once
#include "recipe.h"
#include "IItemComparable.h"
#include "array/fastlist.h"
struct shapelessRecipe : recipe
{
	fastList<IItemComparable*>* requiredItems = nullptr;
	virtual ~shapelessRecipe() override;
	virtual bool verifyPattern(rectangularSlotContainer* container) override;
};