#include "brewingStandSlotContainer.h"
#include "resourcePack.h"
#include "folderList.h"
#include "human.h"
#include "dimension.h"
#include "stackDivider.h"
brewingStandSlotContainer::brewingStandSlotContainer()
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / std::wstring(L"brewing_stand.png"));
	containers.push_back(blazePowderSlot = new uiSlotContainer(cveci2(17, 133), nullptr));
	containers.push_back(ingredientSlot = new uiSlotContainer(cveci2(79, 133), nullptr));
	containers.push_back(potionSlots[0] = new uiSlotContainer(cveci2(56, 99), nullptr));
	containers.push_back(potionSlots[1] = new uiSlotContainer(cveci2(79, 92), nullptr));
	containers.push_back(potionSlots[2] = new uiSlotContainer(cveci2(102, 99), nullptr));
	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);
}

bool brewingStandSlotContainer::addStack(itemStack& stack)
{
	if (stack.count == 0)
	{
		return false;
	}
	if (stack.stackItemID == itemID::blaze_powder)
	{
		return blazePowderSlot->addStack(stack);
	}
	else if (stack.stackItemID == itemID::potion)
	{
		for (int i = 0; i < brewingStandPotionCapacity; i++)
		{
			if (potionSlots[i]->addStack(stack))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return ingredientSlot->addStack(stack);
	}
}

void brewingStandSlotContainer::drawExtraData(cmat3x3& transform, const texture& renderTarget)
{
	//draw fire and progress bar
	//draw fire
	if (selectedBrewingStandData->ticksBrewed)
	{
		cfp& brewProgress = selectedBrewingStandData->ticksBrewed / (fp)brewingBatchTime;
		cint& brewTextureHeight = 27;
		cint& brewTexturePart = (int)(brewTextureHeight * brewProgress);
		inventory::drawExtraData(crectanglei2(176, (227 + brewTextureHeight) - brewTexturePart, 33, brewTexturePart), veci2(97, 121 + (brewTextureHeight - brewTexturePart)), transform, renderTarget);
	}
	//draw progress bar
	if (selectedBrewingStandData->fuelLeft)
	{
		cfp& partLeft = selectedBrewingStandData->fuelLeft / (fp)blazePowderBrewingBatchCount;
		inventory::drawExtraData(crectanglei2(176, 223, (int)(18 * partLeft), 4), veci2(60, 118), transform, renderTarget);
	}
}

void brewingStandSlotContainer::clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot)
{
	//reset process when adding bottle

	inventory::clickedOnItem(button, divider, selectedSlotContainer, selectedSlot);

	human* currentHuman = (human*)linkedPlayer;
	currentHuman->selectedContainerContainer->addUpdatePosition(currentHuman->selectedContainerPosition);
}

brewingStandSlotContainer::~brewingStandSlotContainer()
{
	delete selectedBrewingStandData;
	delete blazePowderSlot;
	for (int i = 0; i < brewingStandPotionCapacity; i++)
	{
		delete potionSlots[i];
	}
	delete hotbarSlots;
	delete inventorySlots;
}

bool brewingStandSlotContainer::canAddStack(uiSlotContainer* containerToAddTo, stackDivider& s)
{
	if (containerToAddTo == blazePowderSlot)
	{
		return s.originalStack.stackItemID == itemID::blaze_powder;
	}
	else
	{
		return inventory::canAddStack(containerToAddTo, s);
	}
}