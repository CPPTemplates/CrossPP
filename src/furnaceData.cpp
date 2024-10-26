#include "furnaceData.h"
#include "tickableBlockContainer.h"
#include "itemData.h"
#include "experienceOrb.h"
#include "nbt/nbtSerializer.h"
#include "math/random/random.h"
furnaceData::furnaceData()
{
	inputSlot = new rectangularSlotContainer(cveci2(1));
	outputSlot = new rectangularSlotContainer(cveci2(1));
	fuelSlot = new rectangularSlotContainer(cveci2(1));
}

bool furnaceData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	cint& burningRate = (containerIn->getBlockID(position) == blockID::furnace) ? 1 : 2;
	cbool burning = ticksFuelBurned < ticksFuelToBurn;
	if (burning)
	{
		ticksFuelBurned += burningRate;
	}
	if (ticksFuelBurned >= ticksFuelToBurn)
	{
		ticksFuelBurned = 0;
		//check:
		//if a recipe needs to be completed
		if (currentRecipe &&
			//if there is something in the fuel slot
			fuelSlot->slots[0].count && itemList[fuelSlot->slots[0].stackItemID]->fuelTicks > 0 &&
			//if there is room left
			((outputSlot->slots[0].count == 0) || ((outputSlot->slots[0].stackItemID == currentRecipe->result.stackItemID) && (outputSlot->slots[0].count < itemList[outputSlot->slots[0].stackItemID]->maxStackSize))))
		{
			//throw new fuel on the fire
			ticksFuelToBurn = itemList[fuelSlot->slots[0].stackItemID]->fuelTicks;

			//remove fuel from slot
			fuelSlot->slots[0].add(-1);
		}
		else
		{
			//no new fuel
			ticksFuelToBurn = 0;
		}
	}

	if (currentRecipe)
	{
		if (burning)
		{
			ticksCooked += burningRate;
			if (ticksCooked >= currentRecipe->cookingTime)
			{
				//move item
				itemStack stack = currentRecipe->result;
				if (outputSlot->slots[0].addStack(stack))
				{
					inputSlot->slots[0].add(-1);
					collectedExperience += currentRecipe->experience;
					if (inputSlot->slots[0].count == 0)
					{
						currentRecipe = nullptr;
					}
				}

				ticksCooked = 0;
			}
		}
		else
		{
			if (ticksCooked > 0)//become cold
			{
				ticksCooked--;
			}
		}
	}
	else
	{
		//burned out
		if (!burning)
		{
			return false;//no need to tick this block anymore
		}
	}
	cbool& newBurning = ticksFuelBurned < ticksFuelToBurn;

	if (newBurning != burning)
	{
		containerIn->addUpdatePosition(position);
	}

	return newBurning || ticksCooked;
}

void furnaceData::onBlockRemove(tickableBlockContainer* containerIn, cveci2& position)
{
	//convert slots to floating slots
	cvec2 floatingSlotPosition = (cvec2)position + cvec2(0.5, 0);
	inputSlot->dropContent(containerIn, floatingSlotPosition);
	outputSlot->dropContent(containerIn, floatingSlotPosition);
	fuelSlot->dropContent(containerIn, floatingSlotPosition);
	dropExperienceOrbs(containerIn, floatingSlotPosition, roundRandom(currentRandom, collectedExperience));
}

furnaceData::~furnaceData()
{
	delete inputSlot;
	delete outputSlot;
	delete fuelSlot;
}

void furnaceData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	inputSlot->serialize(s, std::wstring(L"input slot"));
	outputSlot->serialize(s, std::wstring(L"output slot"));
	fuelSlot->serialize(s, std::wstring(L"fuel slot"));
	s.serializeValue(std::wstring(L"ticks cooked"), ticksCooked);
	s.serializeValue(std::wstring(L"ticks burned"), ticksFuelBurned);
	s.serializeValue(std::wstring(L"ticks to burn"), ticksFuelToBurn);
	s.serializeValue(std::wstring(L"collected experience"), collectedExperience);
	if (!s.write)
	{
		this->currentRecipe = findRecipe(furnaceRecipes, inputSlot);
	}
}