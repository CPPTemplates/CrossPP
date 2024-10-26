#include "smithingTableSlotContainer.h"
#include "resourcePack.h"
#include "folderList.h"
#include "idAnalysis.h"
#include "human.h"
#include "soundList.h"
#include "dimension.h"
#include "stackDivider.h"

smithingTableSlotContainer::smithingTableSlotContainer()
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / std::wstring(L"smithing.png"));
	containers.push_back(smithingInputSlot = new uiSlotContainer(cveci2(27, 103), new rectangularSlotContainer(cveci2(1))));
	containers.push_back(netheriteIngotSlot = new uiSlotContainer(cveci2(76, 103), new rectangularSlotContainer(cveci2(1))));
	containers.push_back(craftingOutputSlot = new uiSlotContainer(cveci2(134, 103), new rectangularSlotContainer(cveci2(1))));

	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);

	craftingInputContainers.push_back(smithingInputSlot);
	craftingInputContainers.push_back(netheriteIngotSlot);
}

bool smithingTableSlotContainer::addStack(itemStack& stack)
{
	if (isMaterialArmor(stack.stackItemID))
	{
		return smithingInputSlot->addStack(stack);
	}
	else if (stack.stackItemID == itemID::netherite_ingot)
	{
		return netheriteIngotSlot->addStack(stack);
	}
	return false;
}

smithingTableSlotContainer::~smithingTableSlotContainer()
{
	delete inventorySlots;
	delete hotbarSlots;

	delete smithingInputSlot;

	delete netheriteIngotSlot;

	delete craftingOutputSlot;
}

itemStack smithingTableSlotContainer::calculateRecipeResult()
{
	const itemStack& inputSlot = smithingInputSlot->linkedContainer->slots[0];

	itemStack resultingStack;

	if (netheriteIngotSlot->linkedContainer->slots[0].count && inputSlot.count)
	{
		resultingStack = inputSlot;
		if (isMaterialArmor(inputSlot.stackItemID))
		{
			resultingStack.stackItemID = getMaterialArmor(getMaterialArmorType(inputSlot.stackItemID), netheriteArmorTier);
		}
		else
		{
			resultingStack.stackItemID = getMaterialTool(getMaterialToolType(inputSlot.stackItemID), netheriteHarvestTier);
		}
	}
	else
	{
		resultingStack = itemStack();
	}

	return resultingStack;
}

bool smithingTableSlotContainer::canAddStack(uiSlotContainer* containerToAddTo, stackDivider& s)
{
	if (containerToAddTo == smithingInputSlot)
	{
		return isMaterialArmor(s.originalStack.stackItemID) || isMaterialTool(s.originalStack.stackItemID);
	}
	else if (containerToAddTo == netheriteIngotSlot)
	{
		return s.originalStack.stackItemID == itemID::netherite_ingot;
	}
	return true;
}

void smithingTableSlotContainer::substractCraftingIngredients()
{
	craftableSlotContainer::substractCraftingIngredients();
	human* currentHuman = (human*)linkedPlayer;
	smithingTableUseSound->playRandomSound(currentHuman->selectedContainerContainer, currentHuman->selectedContainerPosition + cvec2(0.5, 1));
}