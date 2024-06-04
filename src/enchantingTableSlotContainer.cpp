#include "enchantmentData.h"
#include "itemData.h"
#include "application/control/control.h"
#include "lootTable.h"
#include "experienceOrb.h"
#include "resourcePack.h"
#include "enchantingTableSlotContainer.h"
#include "human.h"
#include "folderList.h"
#include "experienceOrb.h"
#include "dimension.h"
#include "soundList.h"
#include "incompatibleEnchantments.h"
#include "inventoryForm.h"
#include "application/theme.h"

constexpr veci2 enchantmentOptionSize = cveci2(108, 19);
constexpr veci2 enchantmentBottomOptionPos = cveci2(60, 95);

enchantingTableSlotContainer::enchantingTableSlotContainer()
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / std::wstring(L"enchanting_table.png"));
	containers.push_back(enchantmentSlot = new uiSlotContainer(cveci2(15, 103), new rectangularSlotContainer(cveci2(1))));
	containers.push_back(lapisSlot = new uiSlotContainer(cveci2(35, 103), new rectangularSlotContainer(cveci2(1))));
	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);
}

bool enchantingTableSlotContainer::addStack(itemStack& stack)
{
	return stack.stackItemID == itemID::lapis_lazuli ? lapisSlot->addStack(stack) : enchantmentSlot->addStack(stack);
}

enchantingTableSlotContainer::~enchantingTableSlotContainer()
{

	delete enchantmentSlot;
	delete inventorySlots;
	delete hotbarSlots;
}

void enchantingTableSlotContainer::mouseDown(cveci2& pixelPosition, cmb& button, stackDivider& divider)
{
	human* currentHuman = (human*)linkedPlayer;
	constexpr rectangle2 optionsRect = crectangle2(enchantmentBottomOptionPos.getX(), enchantmentBottomOptionPos.getY(), enchantmentOptionSize.getX(), enchantmentOptionSize.getY() * enchantmentOptionCount);
	if (optionsRect.contains(pixelPosition) && hasEnchantableItem())
	{
		//try to enchant
		int selectedOptionIndex = (enchantmentOptionCount - 1) - ((pixelPosition.y - enchantmentBottomOptionPos.y) / enchantmentOptionSize.y);
		recalculateEnchantments();
		if (this->isAvailable(selectedOptionIndex))
		{
			//substract experience
			cfp& oldLevel = getExperienceLevel(currentHuman->experience);
			cint& levels = selectedOptionIndex + 1;
			cfp& newLevel = oldLevel - levels;
			currentHuman->experience = (int)getExperience(newLevel);

			itemStack enchantedSlot = enchantmentSlot->linkedContainer->slots[0];
			enchantmentSlot->linkedContainer->slots[0].add(-1);

			enchantedSlot.count = 1;

			for (size_t i = 0; i < enchantmentOptions[selectedOptionIndex].size(); i++)
			{
				enchantedSlot.enchantments.push_back(new enchantment(enchantmentOptions[selectedOptionIndex][i]));
			}

			if (enchantedSlot.stackItemID == itemID::book)
			{
				//drop the enchanted book if the amount is more than one
				enchantedSlot.stackItemID = itemID::enchanted_book;
			}

			if (!enchantmentSlot->linkedContainer->addStack(enchantedSlot))
			{
				lootTable::dropLoot({ enchantedSlot }, currentHuman->selectedContainerContainer, cvec2(currentHuman->selectedContainerPosition) + cvec2(0.5, enchantingTableHeight), maxFloatingSlotSpeed);
			}
			enchantedSlot.clearData();

			enchantSound->playRandomSound(currentHuman->dimensionIn, currentHuman->selectedBlockPosition + cvec2(0.5));
			lapisSlot->linkedContainer->slots[0].add(-levels);//substract lapis
			//generate new enchantment seed
			enchantmentSeed = getmicroseconds();
		}
	}
	else
	{
		inventory::mouseDown(pixelPosition, button, divider);
	}
}

void enchantingTableSlotContainer::drawExtraData(cmat3x3& transform, const texture& renderTarget)
{
	veci2 optionPosition = cveci2(enchantmentBottomOptionPos + cveci2(0, enchantmentOptionSize.y * (enchantmentOptionCount - 1)));
	if (hasEnchantableItem())
	{
		recalculateEnchantments();//you never know if it changed
		//draw enchantments
		for (int i = 0; i < enchantmentOptionCount; i++, optionPosition.y -= enchantmentOptionSize.y)
		{
			cbool available = isAvailable(i);
			rectanglei2 textRect = rectanglei2(optionPosition, enchantmentOptionSize);
			crectanglei2& textBackgroundBrushRect = crectanglei2(0, available ? 71 : 52, 108, 19);
			inventory::drawExtraData(crectanglei2(0, available ? 71 : 52, 108, 19), textRect.pos0, transform, renderTarget);
			textRect.x += textRect.h;//substract the 'lapis square'
			textRect.w -= textRect.h;
			crectangle2& optionBrushRect = crectangle2(i * 0x10, available ? 0x10 : 0, 0x10, 0x10);
			inventory::drawExtraData(crectanglei2(i * 0x10, available ? 0x10 : 0, 0x10, 0x10), optionPosition, transform, renderTarget);

			textRect.expand(-defaultTheme().borderSize);
			std::wstring enchantmentName = enchantmentOptions[i][0].toWString();
			defaultTheme().font->DrawString(enchantmentName, transform.multRectMatrix((crectangle2)textRect), renderTarget);
		}
	}
}

bool enchantingTableSlotContainer::hasEnchantableItem() const
{
	return enchantmentSlot->linkedContainer->slots[0].count && itemList[enchantmentSlot->linkedContainer->slots[0].stackItemID]->enchantability;
}

//call only if there is an item with enchantability
void enchantingTableSlotContainer::recalculateEnchantments()
{
	human* currentHuman = (human*)linkedPlayer;
	itemData* itemToEnchant = itemList[enchantmentSlot->linkedContainer->slots[0].stackItemID];

	std::mt19937 enchantmentRandom = getRandomFromSeed(enchantmentSeed);
	cint bookShelfCount = countBookShelves(currentHuman->selectedContainerContainer, currentHuman->selectedContainerPosition);
	cint bookShelfCount3D = bookShelfCount * 15 / 4;
	cint baseEnchantingLevel = rand(enchantmentRandom, 1, 8) + (int)floor(bookShelfCount3D / 2.0) + rand(enchantmentRandom, bookShelfCount3D);

	baseEnchantmentLevels[0] = math::maximum(baseEnchantingLevel / enchantmentOptionCount, 1),
		baseEnchantmentLevels[1] = ((baseEnchantingLevel * 2) / enchantmentOptionCount) + 1;
	baseEnchantmentLevels[2] = math::maximum(baseEnchantingLevel, bookShelfCount3D * 2);


	cint enchantability = itemToEnchant->enchantability;

	for (int enchantmentOptionIndex = 0; enchantmentOptionIndex < enchantmentOptionCount; enchantmentOptionIndex++)
	{
		enchantmentOptions[enchantmentOptionIndex] = std::vector<enchantment>();

		//https://minecraft.gamepedia.com/Enchanting_mechanics#Step_one_.E2.80.93_Applying_modifiers_to_the_enchantment_level

		// Generate a random number between 1 and 1+(enchantability/2), with a triangular distribution
		cint randomEnchantability = 1 + rand(enchantmentRandom, enchantability / 4) + rand(enchantmentRandom, enchantability / 4);

		// Choose the enchantment level
		cint k = baseEnchantmentLevels[enchantmentOptionIndex] + randomEnchantability;

		// A random bonus, between .85 and 1.15
		cfp rand_bonus_percent = 1 + (randFp(enchantmentRandom) + randFp(enchantmentRandom) - 1) * 0.15;

		// Finally, we calculate the level
		int finalLevel = math::maximum((int)round(k * rand_bonus_percent), 1);

		fastList<enchantment> enchantmentsToChooseFrom = fastList<enchantment>();

		int totalWeight = 0;

		for (enchantmentID identifier : itemToEnchant->possibleEnchantments)
		{
			enchantmentData* currentEnchantmentData = enchantmentDataList[(int)identifier];
			//the power of 'sharpness 5' is 5 for example
			for (int enchantmentPower = 1; enchantmentPower <= (int)currentEnchantmentData->powerRanges.size; enchantmentPower++)
			{
				if (currentEnchantmentData->powerRanges[enchantmentPower - 1].contains(cveci1(finalLevel)))
				{
					if (enchantmentPower >= (int)currentEnchantmentData->powerRanges.size || !currentEnchantmentData->powerRanges[enchantmentPower].contains(cveci1(finalLevel)))
					{
						//this is the highest enchantment power power, choose this
						enchantmentsToChooseFrom.push_back(enchantment(identifier, enchantmentPower));
						totalWeight += currentEnchantmentData->weight;
						break;
					}
				}
			}
		}
		enchantmentsToChooseFrom.update();
		//choose from the list
		if (enchantmentsToChooseFrom.size)
		{
		bonusRoll:
			int value = rand(enchantmentRandom, totalWeight - 1);
			for (size_t choiceIndex = 0; choiceIndex < enchantmentsToChooseFrom.size; choiceIndex++)
			{
				value -= enchantmentDataList[(int)enchantmentsToChooseFrom[choiceIndex].identifier]->weight;
				if (value < 0)
				{
					enchantmentOptions[enchantmentOptionIndex].push_back(enchantmentsToChooseFrom[choiceIndex]);

					//remove all incombinable enchantments from the list

					for (auto it = enchantmentsToChooseFrom.begin(); it != enchantmentsToChooseFrom.end(); it++)
					{
						if ((it->identifier == enchantmentsToChooseFrom[choiceIndex].identifier) || (!canCombine(it->identifier, enchantmentsToChooseFrom[choiceIndex].identifier)))
						{
							enchantmentsToChooseFrom.erase(it);
						}
					}
					enchantmentsToChooseFrom.update();
					break;
				}
			}
			if (randIndex(enchantmentRandom, 50) <= finalLevel)
			{
				finalLevel /= 2;
				goto bonusRoll;
			}
		}
	}
}

//dont call this if there is no enchantable item in the itemslot
bool enchantingTableSlotContainer::isAvailable(cint& optionIndex) const
{
	human* currentHuman = (human*)linkedPlayer;
	return lapisSlot->linkedContainer->slots[0].count >= 1 + optionIndex &&
		enchantmentSlot->linkedContainer->slots[0].enchantments.size() == 0 &&
		getExperienceLevel(currentHuman->experience) >= baseEnchantmentLevels[optionIndex];
}

int enchantingTableSlotContainer::countBookShelves(blockContainer* containerIn, cveci2& position)
{
	//bookshelves required:

	//b   b
	//b t b
	int bookshelfCount = 0;
	std::vector<veci2> checkPositions = { veci2(-2,0),veci2(-2,1),veci2(2,0),veci2(2,1) };
	for (veci2 relativePosition : checkPositions)
	{
		if (containerIn->getBlockID(position + relativePosition) == blockID::bookshelf)
		{
			bookshelfCount++;
		}
	}
	return bookshelfCount;
}