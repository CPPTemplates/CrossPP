#include "floatingSlot.h"
#include "spawnerData.h"
#include "minecart.h"
#include "idAnalysis.h"
#include "itemData.h"
#include "dispenserData.h"
#include "rectangularSlotContainer.h"
#include "tickableBlockContainer.h"
#include "array/arrayFunctions/arrayFunctions.h"
#include "soundList.h"
#include "entityData.h"
#include "entity.h"
#include "block.h"
#include "mobList.h"
#include "nbt/nbtSerializer.h"
#include "constants/vectorConstants.h"
dispenserData::dispenserData(const directionID& directionFacing) :facingData(directionFacing)
{
	slots = new rectangularSlotContainer(dropperSlotContainerSize);
}

void dispenserData::onBlockRemove(tickableBlockContainer* containerIn, cveci2& position)
{
	//convert slots to floating slots
	cvec2 floatingSlotPosition = (cvec2)position + cvec2(0.5, 0);
	slots->dropContent(containerIn, floatingSlotPosition);
}

dispenserData::~dispenserData()
{

	delete slots;
}

void dispenserData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	facingData::serializeValue(s);
	slots->serialize(s, std::wstring(L"slots"));
	s.serializeValue(std::wstring(L"was powered"), wasPowered);
}

bool dispenserData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	const powerLevel& level = containerIn->getPowerLevel(position);
	if (!wasPowered)
	{
		if (level > 0)
		{
			bool dispenseSucceded = false;
			//dispense a random item

			std::vector<fp> weights = std::vector<fp>(slots->rowsAndColumns.volume());
			for (int i = 0; i < slots->rowsAndColumns.volume(); i++)
			{
				weights[i] = slots->slots[i].count ? (slots->slots[i].count / (fp)itemList[slots->slots[i].stackItemID]->maxStackSize) : 0;
			}
			cfp& sum = getSum<fp>(weights);
			if (sum > 0)
			{
				//choose item
				csize_t& index = randIndex(currentRandom, weights, sum);
				if (dispenseItem(slots->slots[index], containerIn, position))
				{
					dispenseSucceded = true;
					slots->slots[index].add(-1);
				}
			}
			clickSound->playRandomSound(containerIn, cvec2(position) + cvec2(0.5), 1, dispenseSucceded ? 0.5 : 1);
		}
	}
	wasPowered = level > 0;
	return false;
}

entity* dispenserData::summonEntityTouching(const entityID& entityType, tickableBlockContainer* containerIn, cveci2& position)
{
	cveci2 directionNormal = directionVectors2D[(int)directionFacing];

	const axisID& axis = getAxis(directionFacing);

	crectangle2 initialRelativeHitbox = entityDataList[(int)entityType]->initialRelativeHitbox;

	cvec2 initialRelativeHitboxCenter = initialRelativeHitbox.getCenter();

	cfp offset = 0.5 + (initialRelativeHitbox.size[axis] * 0.5);

	cvec2 initialPosition = (cvec2(position) + cvec2(0.5) + (cvec2(directionNormal) * offset)) + (initialRelativeHitbox.pos0 - initialRelativeHitboxCenter);
	crectangle2 initialHitbox = crectangle2(initialRelativeHitbox.pos0 + initialPosition, initialRelativeHitbox.size)
		.touchingBorder(crectangle2(position, cvec2(1)), directionFacing);

	if (containerIn->getHitboxCollisionType(initialHitbox) != collisionTypeID::willCollide)
	{
		return summonEntity(entityType, containerIn, initialHitbox.pos0 - initialRelativeHitbox.pos0);
	}
	else
	{
		return nullptr;
	}
}

bool dispenserData::dispenseItem(const itemStack& stack, tickableBlockContainer* containerIn, cveci2& position)
{
	cveci2 adjacentBlockPosition = position + directionVectors2D[(int)directionFacing];
	if (isMinecartItem(stack.stackItemID))
	{
		if (isRail(containerIn->getBlockID(adjacentBlockPosition)))
		{
			cvec2 mineCartPos00 = cvec2(adjacentBlockPosition) + cvec2(0.5, sleepersHeight + math::fpepsilon);
			entity* summonedEntity = summonEntity(entityID::minecart, containerIn, mineCartPos00);
			if (summonedEntity)
			{
				((minecart*)summonedEntity)->addCollidingEntities();
				return true;
			}
		}
	}
	else if ((blockID)stack.stackItemID == blockID::tnt)
	{
		if (containerIn->getBlock(adjacentBlockPosition)->canReplaceBlock)
		{
			containerIn->setBlockID(adjacentBlockPosition, blockID::tnt);
			containerIn->fuseTNT(adjacentBlockPosition);
		}
		return true;
	}
	else if (isSpawnEgg(stack.stackItemID))
	{
		const entityID mobToSpawn = mobList[(int)stack.stackItemID - (int)itemID::spawn_egg];
		if (containerIn->getBlockID(adjacentBlockPosition) == blockID::spawner)
		{
			dynamic_cast<spawnerData*>(containerIn->getBlockData(adjacentBlockPosition))->entityToSpawn = mobToSpawn;
		}
		else
		{
			entity* summonedEntity = summonEntityTouching(mobToSpawn, containerIn, position);
			if (summonedEntity)
			{
				return true;
			}
		}

	}
	return dropItem(stack, containerIn, position);
}

bool dispenserData::dropItem(const itemStack& stack, tickableBlockContainer* containerIn, cveci2& position)
{
	cveci2 directionNormal = directionVectors2D[(int)directionFacing];
	entity* summonedEntity = summonEntityTouching(entityID::item, containerIn, position);
	cfp& throwPower = containerIn->getPowerLevel(position) / (fp)maxPowerLevel;
	if (summonedEntity)
	{
		floatingSlot* slot = (floatingSlot*)summonedEntity;
		slot->speed = cvec2(directionNormal) * throwPower * maxFloatingSlotSpeed;
		slot->stack = itemStack(stack.stackItemID, 1, createItemTag(stack.stackItemID));
		return true;
	}
	else
	{
		return false;
	}
}