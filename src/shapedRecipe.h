#pragma once
#include "recipe.h"
#include "IItemComparable.h"
#include "array/arraynd/arraynd.h"
struct shapedRecipe : recipe
{
	array2d<IItemComparable*>* requiredPattern = nullptr;
	virtual ~shapedRecipe() override;
	virtual bool verifyPattern(rectangularSlotContainer* container) override;
};