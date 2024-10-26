#include "shapeLessRecipe.h"
shapelessRecipe::~shapelessRecipe()
{

	delete requiredItems;
}

bool shapelessRecipe::verifyPattern(rectangularSlotContainer* container)
{
	bool* found = new bool[requiredItems->size]();
	for (int j = 0; j < container->rowsAndColumns.y; j++)
	{
		for (int k = 0; k < container->rowsAndColumns.x; k++)
		{
			itemStack* s = container->getSlot(cveci2(j, k));
			if (s->count)
			{
				for (size_t i = 0; i < requiredItems->size; i++)
				{
					IItemComparable* comparable = (*requiredItems)[i];
					if (comparable->compare(s->stackItemID))
					{
						if (!found[i]) //can't have too much of the same item
						{
							found[i] = true;
							goto itemBelongs;
						}
					}
				}
				delete[] found;//wrong item
				return false;
			itemBelongs:;
			}
		}
	}
	for (size_t i = 0; i < requiredItems->size; i++)
	{
		if (!(found[i]))
		{
			delete[] found;
			return false;
		}
	}
	delete[] found;
	return true;
}