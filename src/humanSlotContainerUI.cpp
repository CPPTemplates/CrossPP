#include "humanSlotContainerUI.h"
#include "resourcePack.h"
#include "folderList.h"
#include "human.h"
#include "idAnalysis.h"
#include "dimension.h"
#include "stackDivider.h"
humanSlotContainerUI::humanSlotContainerUI() : craftingGridSlotContainer(new uiSlotContainer(cveci2(98, 113), new rectangularSlotContainer(cveci2(2))))
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / std::wstring(L"inventory.png"));
	containers.push_back(hotbarSlots = new uiSlotContainer(cveci2(0x8, 0x8)));
	containers.push_back(inventorySlots = new uiSlotContainer(cveci2(0x8, 30)));
	containers.push_back(armorSlots = new uiSlotContainer(cveci2(0x8, 87)));
	containers.push_back(leftHandSlot = new uiSlotContainer(cveci2(77, 87)));
	containers.push_back(craftingSlots);
	containers.push_back(craftingOutputSlot = new uiSlotContainer(cveci2(154, 122), new rectangularSlotContainer(cveci2(1))));
}

void humanSlotContainerUI::clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot)
{
	human* currentHuman = (human*)linkedPlayer;
	if (selectedSlotContainer == armorSlots)
	{
		if (divider.originalStack.count)
		{
			//check if it is the right armor(piece)
			if (getArmorTier(divider.originalStack.stackItemID) == noArmorTier)
			{
				return;
			}
			else if ((getArmorType(divider.originalStack.stackItemID) - bootsArmorType) != selectedSlot.y)
			{
				return;
			}
			else
			{
				getEquipSound(divider.originalStack.stackItemID)->playRandomSound(linkedPlayer->dimensionIn, currentHuman->getHeadPosition());
			}
		}
		else
		{
			itemStack& stack = selectedSlotContainer->linkedContainer->slots[selectedSlot.y];
			if (stack.count && stack.getEnchantmentLevel(enchantmentID::curseOfBinding))
			{
				return;
			}
		}
	}
	craftableSlotContainer::clickedOnItem(button, divider, selectedSlotContainer, selectedSlot);
}