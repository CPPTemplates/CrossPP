#include "enchantmentData.h"
#include "idAnalysis.h"
#include "resourcePack.h"
#include "anvilSlotContainer.h"
#include "folderList.h"
#include "itemData.h"
#include "durabilityData.h"
#include "incompatibleEnchantments.h"
#include "human.h"
#include "soundList.h"
#include <numeric>
#include <string>
#include <vector>
#include "enchantment.h"
#include "enchantmentID.h"
#include "globalFunctions.h"
#include "math/mathFunctions.h"
#include "math/vector/vectn.h"
#include "itemID.h"
#include "itemStack.h"
#include "rectangularSlotContainer.h"
#include "uiSlotContainer.h"
#include "playerSocket.h"

anvilSlotContainer::anvilSlotContainer()
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / std::wstring(L"anvil.png"));

	craftingInputContainers.push_back(new uiSlotContainer(cveci2(27, 103), new rectangularSlotContainer(cveci2(1))));
	craftingInputContainers.push_back(new uiSlotContainer(cveci2(76, 103), new rectangularSlotContainer(cveci2(1))));

	containers.push_back(craftingOutputSlot = new uiSlotContainer(cveci2(134, 103), new rectangularSlotContainer(cveci2(1))));
	containers.push_back(craftingInputContainers[0]);
	containers.push_back(craftingInputContainers[1]);

	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);
}

bool anvilSlotContainer::addStack(itemStack& stack)
{
	return false;
}

anvilSlotContainer::~anvilSlotContainer()
{
	delete inventorySlots;
	delete hotbarSlots;
	delete craftingOutputSlot;
	delete craftingInputContainers[0];
	delete craftingInputContainers[1];
}

itemStack anvilSlotContainer::calculateRecipeResult()
{
	const itemStack inputStacks[2] =
	{
		craftingInputContainers[0]->linkedContainer->slots[0],
		craftingInputContainers[1]->linkedContainer->slots[0]
	};

	itemsUsed = 1;

	if ((int)inputStacks[0].stackItemID && (int)inputStacks[1].stackItemID)
	{
		//combine
		if ((inputStacks[0].stackItemID == inputStacks[1].stackItemID) || (itemList[inputStacks[1].stackItemID]->possibleEnchantments.size() && (inputStacks[1].stackItemID == itemID::enchanted_book)))
		{
			itemStack resultingStack = inputStacks[0];

			if (hasDurability(inputStacks[0].stackItemID) && hasDurability(inputStacks[1].stackItemID))
			{
				//combine durability
				constexpr fp durabilityBonus = 0.12;
				((durabilityData*)resultingStack.data)->durability = math::minimum(((durabilityData*)inputStacks[0].data)->durability + ((durabilityData*)inputStacks[1].data)->durability + durabilityBonus, (fp)1);
			}

			//combine enchantments

			const std::vector<enchantmentID>& possibleEnchantments = itemList[inputStacks[0].stackItemID]->possibleEnchantments;

			for (size_t enchantmentIndex1 = 0; enchantmentIndex1 < inputStacks[1].enchantments.size(); enchantmentIndex1++)
			{
				if (std::find(possibleEnchantments.begin(), possibleEnchantments.end(), inputStacks[1].enchantments[enchantmentIndex1]->identifier) != possibleEnchantments.end())
				{
					//check if the enchantment can be combined
					for (size_t enchantmentIndex0 = 0; enchantmentIndex0 < inputStacks[0].enchantments.size(); enchantmentIndex0++)
					{
						if (inputStacks[0].enchantments[enchantmentIndex0]->identifier == inputStacks[1].enchantments[enchantmentIndex1]->identifier)
						{
							cint& level0 = inputStacks[0].enchantments[enchantmentIndex0]->power;
							cint& level1 = inputStacks[1].enchantments[enchantmentIndex1]->power;
							cint& maxLevel = enchantmentDataList[inputStacks[0].enchantments[enchantmentIndex0]->identifier]->maxLevel;

							//the index will always be the same, as additional enchantments will be added to the back of the list
							//add one level if the levels are equal
							resultingStack.enchantments[enchantmentIndex0]->power = (level0 < level1) ? level1 : ((level1 < level0) ? level0 : math::minimum(level0 + 1, maxLevel));
							//no need to check further, because the enchantment is found on both items
							goto dontAdd;
						}
						else
						{
							if (!canCombine(inputStacks[0].enchantments[enchantmentIndex0]->identifier, inputStacks[1].enchantments[enchantmentIndex1]->identifier))
							{
								goto dontAdd;
							}
						}
					}
					resultingStack.enchantments.push_back(new enchantment(inputStacks[1].enchantments[enchantmentIndex1]->clone<enchantment>()));
				}
				//the item in the left slot 'wins'
			dontAdd:;
			}
			return resultingStack;
		}
		else if (isRepairItem(inputStacks[0].stackItemID, inputStacks[1].stackItemID))
		{
			itemStack resultingStack = inputStacks[0];

			for (itemsUsed = 0; (itemsUsed < inputStacks[1].count) && (((durabilityData*)resultingStack.data)->durability < 1); itemsUsed++)
			{
				((durabilityData*)resultingStack.data)->durability = math::minimum(((durabilityData*)resultingStack.data)->durability + 0.25, (fp)1);/// * inputStacks[1].count;
			}
			return resultingStack;
		}
	}
	return itemStack();
}

void anvilSlotContainer::substractCraftingIngredients()
{
	craftingInputContainers[0]->linkedContainer->slots[0].add(-1);
	craftingInputContainers[1]->linkedContainer->slots[0].add(-itemsUsed);
	anvilUseSound->playRandomSound(linkedPlayer->selectedContainerContainer, vec2(linkedPlayer->selectedContainerPosition) + vec2(0.5, 1));
}