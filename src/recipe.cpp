#include "filesystem/jsonReader.h"
#include "recipe.h"
#include "shapedRecipe.h"
#include "tag.h"
#include "shapeLessRecipe.h"
#include "furnaceRecipe.h"
#include "itemData.h"
#include "include/optimization/handleError.h"

std::vector<recipe*> craftingRecipes = std::vector<recipe*>();

void readRecipe(const jsonContainer& recipeDescription)
{
	std::wstring recipeType = recipeDescription.children[0].children[0].value;
	recipe* resultingRecipe = nullptr;

	if (recipeType == std::wstring(L"minecraft:crafting_shaped"))
	{
		const jsonContainer& patternContainer = recipeDescription.children[recipeDescription.getChildIndex(std::wstring(L"pattern"))];

		//first string
		cveci2& patternSize = cveci2((int)patternContainer.children[0].value.size(), (int)patternContainer.children.size());

		shapedRecipe* currentRecipe = new shapedRecipe();
		currentRecipe->requiredPattern = new array2d<IItemComparable*>(patternSize, true);
		const jsonContainer& keysContainer = recipeDescription.children[recipeDescription.getChildIndex(std::wstring(L"key"))];
		fastList<IItemComparable*>* keyList = new fastList<IItemComparable*>();
		//read key list
		for (const jsonContainer& keyContainer : keysContainer.children)
		{
			if (keyContainer.children.size() > 1)
			{
				tag* t = new tag(std::wstring());//name does not matter
				for (const jsonContainer& child : keyContainer.children)
				{
					if (IItemComparable* comparable = getItemComparableByName(child.children[0].children[0].value))
					{
						t->taggedComparables->push_back(comparable);
					}
				}
				t->taggedComparables->update();
				if (t->taggedComparables->size)
				{
					keyList->push_back(t);
				}
				else
				{

					delete t;

					delete keyList;

					delete currentRecipe;
					return;
				}
			}
			else
			{
				IItemComparable* comparable = getItemComparableByName(keyContainer.children[0].children[0].value);
				if (comparable == nullptr)
				{

					delete keyList;

					delete currentRecipe;
					return;
				}
				keyList->push_back(comparable);
			}
		}

		keyList->update();
		//fill the pattern
		for (int j = 0; j < patternSize.y; j++)
		{
			for (int i = 0; i < patternSize.x; i++)
			{
				letter currentSymbol = patternContainer.children[j].value[i];
				size_t keyIndex = keysContainer.getChildIndex(std::wstring(1, currentSymbol));
				if (keyIndex != std::wstring::npos)
				{
					//flip recipes vertically
					currentRecipe->requiredPattern->setValue(cveci2(i, patternSize.y - j - 1), (*keyList)[keyIndex]);
				}
			}
		}

		delete keyList;
		resultingRecipe = currentRecipe;
	}
	else if (recipeType == std::wstring(L"minecraft:crafting_shapeless"))
	{
		const jsonContainer& ingredientsContainer = recipeDescription.children[recipeDescription.getChildIndex(std::wstring(L"ingredients"))];
		const std::vector<jsonContainer> ingredientContainerList = ingredientsContainer.children;
		shapelessRecipe* currentRecipe = new shapelessRecipe();
		currentRecipe->requiredItems = new fastList<IItemComparable*>();
		for (const jsonContainer& ingredientContainer : ingredientContainerList)
		{
			std::wstring itemName = ingredientContainer.children[0].children[0].value;
			IItemComparable* comparable = getItemComparableByName(itemName);
			if (comparable == nullptr)
			{

				delete currentRecipe;
				return;
			}
			currentRecipe->requiredItems->push_back(comparable);
		}
		currentRecipe->requiredItems->update();
		resultingRecipe = currentRecipe;
	}
	else if (recipeType == std::wstring(L"minecraft:smelting"))
	{
		furnaceRecipe* currentRecipe = new furnaceRecipe();
		const std::wstring ingredientName = recipeDescription.children[recipeDescription.getChildIndex(std::wstring(L"ingredient"))].children[0].children[0].value;
		currentRecipe->requiredItem = getItemComparableByName(ingredientName);
		if (currentRecipe->requiredItem == nullptr)
		{

			delete currentRecipe;
			return;
		}
		convertToDouble(recipeDescription.children[recipeDescription.getChildIndex(std::wstring(L"experience"))].children[0].value, currentRecipe->experience);
		currentRecipe->cookingTime = stoi(recipeDescription.children[recipeDescription.getChildIndex(std::wstring(L"cookingtime"))].children[0].value);
		resultingRecipe = currentRecipe;
	}
	else
	{
		return;
	}
	//check if the result is valid
	const jsonContainer& resultContainer = recipeDescription.children[recipeDescription.getChildIndex(std::wstring(L"result"))];

	//a layer deeper or not?
	const std::wstring resultItemName = resultContainer.children[0].children.size() ?
		resultContainer.children[0].children[0].value :
		resultContainer.children[0].value;

	const itemID& resultItemIndex = itemList.getIDByName(resultItemName);
	if ((int)resultItemIndex == -1)
	{

		delete resultingRecipe;
		return;
	}
	resultingRecipe->result.stackItemID = resultItemIndex;

	if (resultContainer.children.size() > 1)
	{
		//count
		resultingRecipe->result.count = std::stoi(resultContainer.children[1].children[0].value);
	}
	else
	{
		resultingRecipe->result.count = 1;
	}
	resultingRecipe->result.data = createItemTag(resultingRecipe->result.stackItemID);
	if (recipeType == std::wstring(L"minecraft:smelting"))
	{
		furnaceRecipes.push_back((furnaceRecipe*)resultingRecipe);
	}
	else
	{
		craftingRecipes.push_back(resultingRecipe);
	}
}
void substractOneOfEachSlot(rectangularSlotContainer* container)
{
	veci2 substractPos = veci2();
	//substract 1 from all filled slots in the crafting workspace
	for (substractPos.y = 0; substractPos.y < container->rowsAndColumns.y; substractPos.y++)
	{
		for (substractPos.x = 0; substractPos.x < container->rowsAndColumns.x; substractPos.x++)
		{
			itemStack* craftingSlot = container->getSlot(substractPos);
			if (craftingSlot->count)
			{
				craftingSlot->add(-1);
			}
		}

	}
}
//bool recipe::verifyPattern(rectangularSlotContainer* container)
//{
//	handleError(std::wstring(L"check not implemented"));
//	return false;
//}

recipe::~recipe()
{
}