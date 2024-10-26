#include "rectangularSlotContainer.h"
#include "itemData.h"
#include "lootTable.h"
#include "nbt/nbtSerializer.h"
rectangularSlotContainer::rectangularSlotContainer(cveci2& rowsAndColumns)
{
	this->rowsAndColumns = rowsAndColumns;
	slots = new itemStack[rowsAndColumns.x * rowsAndColumns.y]();//initialize to default value(nullptr)
}

void rectangularSlotContainer::render(const gameRenderData& targetData, cvec2& pos, cfp& offset, cfp& itemDrawSize)
{
	//draw slots
	const itemStack* currentSlot = this->slots;
	for (int j = 0; j < rowsAndColumns.y; j++)
	{
		for (int i = 0; i < rowsAndColumns.x; i++, currentSlot++)
		{
			if (currentSlot->count)
			{
				cvec2& currentPosition = pos + cvec2(i, j) * offset;
				currentSlot->render(crectangle2(currentPosition, cvec2(itemDrawSize)), targetData);
			}
			//renderTarget.fillTexture(crectangle2(slots[i + j * rowsAndColumns.x].item->textureCoords, veci2(blockTextureSize)), crectangle2(offset, cvec2(blockTextureSize)), *blockTextures);
		}
	}
}

bool rectangularSlotContainer::getSlot(cveci2& mousePos, cveci2& drawPos, cint& offset, cint& itemDrawSize, veci2& slotPosition) const
{
	//check which slot is clicked on
	//convert to 'hotbar slot space'
	veci2 relativeMousePosition = mousePos - drawPos;

	if (relativeMousePosition.x >= 0 && relativeMousePosition.y >= 0 && relativeMousePosition.x < offset * (rowsAndColumns.x - 1) + itemDrawSize && relativeMousePosition.y < offset * (rowsAndColumns.y - 1) + itemDrawSize)
	{
		//somewhere clicked
		veci2 selectedItemPosition = veci2(relativeMousePosition.x / offset, relativeMousePosition.y / offset);
		//if clicked on the borders between items, dont select any item.
		veci2 border = selectedItemPosition * offset + itemDrawSize;
		if (selectedItemPosition.x < border.x && selectedItemPosition.y < border.y)
		{
			slotPosition = selectedItemPosition;
			return true;
		}
	}
	return false;
}

//returns true if the itemStack is fully added.
//returns false if there are items left in this itemStack.
bool rectangularSlotContainer::addStack(itemStack& stack)
{
	if (stack.count == 0)
	{
		return false;
	}
	itemStack* slotptr = this->slots;
	itemStack* endptr = slotptr + rowsAndColumns.x * rowsAndColumns.y;
	//use this if there is not enough room in the slots with the same item 
	itemStack* emptySlot = nullptr;
	while (slotptr < endptr)
	{
		//check for room
		//will go wrong at instances, so only use pointers
		if (slotptr->count)
		{
			if (slotptr->addStack(stack))
			{
				return true;
			}
		}
		else if (!emptySlot)
		{
			emptySlot = slotptr;
		}
		slotptr++;
	}
	//no blocks, use this slot as new slot

	if (emptySlot)
	{
		*emptySlot = stack;
		stack.clearData();
		return true;
	}
	else
	{
		return false;
	}
}

int rectangularSlotContainer::countAmount(const itemStack& s) const
{
	const itemStack* const endPtr = slots + rowsAndColumns.x * rowsAndColumns.y;
	int count = 0;
	for (const itemStack* slot = slots; slot < endPtr; slot++)
	{
		if (slot->compare(s))
		{
			count += slot->count;
		}
	}
	return count;
}

bool rectangularSlotContainer::substractStack(itemStack& stack)
{
	if (stack.count == 0)
	{
		return false;
	}
	itemStack* slotptr = this->slots;
	const itemStack* const endptr = slotptr + rowsAndColumns.x * rowsAndColumns.y;
	//use this if there are not enough partially filled slots to substract from
	itemStack* fullSlot = nullptr;
	while (slotptr < endptr)
	{
		//check for room
		//will go wrong at instances, so only use pointers
		if (slotptr->compare(stack))
		{
			cint decreaseCount = math::minimum(stack.count, slotptr->count);
			//use this slot and simply increase the amount
			stack.add(-decreaseCount);
			slotptr->add(-decreaseCount);
		}
		else if (!fullSlot && slotptr->count == itemList[(int)stack.stackItemID]->maxStackSize)
		{
			fullSlot = slotptr;
		}
		if (stack.count == 0)
		{
			return true;
		}
		slotptr++;
	}
	//no blocks, use this slot as new slot

	if (fullSlot)
	{
		fullSlot->add(-stack.count);
		stack.clearData();
		return true;
	}
	else
	{
		return false;
	}
}

rectangularSlotContainer::~rectangularSlotContainer()
{
	delete[] slots;
}

void rectangularSlotContainer::dropContent(tickableBlockContainer* containerIn, cvec2& dropPos, cfp& maxSpeed)
{
	cint slotCount = rowsAndColumns.x * rowsAndColumns.y;

	std::vector<itemStack> containerLoot = std::vector<itemStack>();

	for (int i = 0; i < slotCount; i++)
	{
		if (slots[i].count)
		{
			containerLoot.push_back(slots[i]);
			//no need to clear data
			slots[i] = itemStack();
		}
	}
	lootTable::dropLoot(containerLoot, containerIn, dropPos, maxSpeed);
}

void rectangularSlotContainer::transfer(slotContainer* const containerTo, tickableBlockContainer* containerIn, cvec2& dropPos) const
{
	cint slotCount = rowsAndColumns.x * rowsAndColumns.y;
	for (int i = 0; i < slotCount; i++)
	{
		if (slots[i].count)
		{
			if (!containerTo->addStack(slots[i]))
			{
				lootTable::dropLoot({ slots[i] }, containerIn, dropPos, maxFloatingSlotSpeed);
				slots[i].clearData();
			}
		}
	}
}

void rectangularSlotContainer::serializeValue(nbtSerializer& s)
{
	if (s.push<nbtDataTag::tagList>(std::wstring(L"slots")))
	{
		if (s.write)
		{
			for (int i = 0; i < rowsAndColumns.x * rowsAndColumns.y; i++)
			{
				if (s.push<nbtDataTag::tagCompound>())
				{
					slots[i].serializeValue(s);
					s.pop();
				}
			}
		}
		else
		{
			std::vector<nbtData*> serializedSlots = s.getChildren();
			for (int i = 0; i < rowsAndColumns.x * rowsAndColumns.y; i++)
			{
				if (s.push())
				{
					slots[i].serializeValue(s);
					s.pop();
				}
			}
		}
		s.pop();
	}
}

void rectangularSlotContainer::clearData()
{
	cint slotCount = rowsAndColumns.x * rowsAndColumns.y;
	for (int i = 0; i < slotCount; i++)
	{
		if (slots[i].count)
		{
			slots[i].clearData();
		}
	}
}