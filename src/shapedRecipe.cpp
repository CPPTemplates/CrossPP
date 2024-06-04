#include "shapedRecipe.h"
shapedRecipe::~shapedRecipe()
{
	delete requiredPattern;
}

bool shapedRecipe::verifyPattern(rectangularSlotContainer* container)
{
	cveci2 repetitions = cveci2(container->rowsAndColumns.x - (int)requiredPattern->size.x, container->rowsAndColumns.y - (int)requiredPattern->size.y) + 1;
	if (repetitions.x < 1 || repetitions.y < 1) return false;//the recipe is too big for this workspace
	veci2 currentRepetition = veci2();
	for (currentRepetition.y = 0; currentRepetition.y < repetitions.y; currentRepetition.y++)
	{
		for (currentRepetition.x = 0; currentRepetition.x < repetitions.x; currentRepetition.x++)
		{
			veci2 positionInWorkSpace = veci2();
			for (positionInWorkSpace.y = 0; positionInWorkSpace.y < container->rowsAndColumns.y; positionInWorkSpace.y++)
			{
				for (positionInWorkSpace.x = 0; positionInWorkSpace.x < container->rowsAndColumns.x; positionInWorkSpace.x++)
				{
					cveci2 positionInRecipe = positionInWorkSpace - currentRepetition;
					IItemComparable* requiredItem = requiredPattern->getValue(positionInRecipe);//will return nullptr if outside pattern

					itemID itemToCompare = container->slots[positionInWorkSpace.x + container->rowsAndColumns.x * positionInWorkSpace.y].stackItemID;
					//not the same
					if (requiredItem ? (!requiredItem->compare(itemToCompare)) : ((int)itemToCompare != 0))
					{
						goto itemDoesNotBelong;
					}
				}
			}
			return true;
		itemDoesNotBelong:;
		}
	}
	return false;
}