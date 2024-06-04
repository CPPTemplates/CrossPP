#include "tickableBlockContainer.h"
#include "dropData.h"
#include "lootTable.h"
#include "block.h"
#include "openData.h"
#include "doubleBlockData.h"
#include "soundList.h"
#include "math/collisions.h"
#include "tnt.h"
#include "array/arrayFunctions/arrayFunctions.h"
#include "array/arrayFunctions/sortedArray.h"
#include <memory>
#include <numeric>
#include <set>
#include <vector>
#include "arrayDataType.h"
#include "blockContainer.h"
#include "blockData.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "constants.h"
#include "damageSource.h"
#include "damageType.h"
#include "dimensionID.h"
#include "entity.h"
#include "entityID.h"
#include "facingData.h"
#include "idAnalysis.h"
#include "array/fastarray.h"
#include "globalFunctions.h"
#include "math/direction.h"
#include "math/mathFunctions.h"
#include "math/mattnxn.h"
#include "math/random/random.h"
#include "math/rectangle/rectangletn.h"
#include "math/uuid.h"
#include "math/vector/vectn.h"
#include "itemID.h"
#include "itemStack.h"
#include "levelID.h"
#include "lightLevel.h"
#include "lightLevelID.h"
#include "dimension.h"
#include <math/raycast.h>
#include "dimensionData.h"

constexpr int decayStrength[(size_t)levelID::count][directionCount2D]
{
	{0xc, 0xc, 0, 0x20},
	{0x8, 0x8, 0x8, 0x8},
	{0x0, 0x0, 0x0, 0x0},
};

void tickableBlockContainer::harvestBlock(dropData data, cveci2& position, const chunkLoadLevel& minimalLoadLevel)
{
	block* b = getBlock(position);
	if (b->canHarvest(data.toolUsed ? data.toolUsed->stackItemID : (itemID)0))
	{
		data.harvestedBlockData = getBlockData(position);
		const std::vector<itemStack> drops = b->dropsWhenHarvested->roll(data, currentRandom);
		lootTable::dropLoot(drops, this, cvec2(position) + cvec2(0.5, 0), maxFloatingSlotSpeed);
	}
}
void tickableBlockContainer::removeBlock(cveci2& position, const blockID& oldBlockID, blockData* const& oldData, const chunkLoadLevel& minimalLoadLevel)
{
	if (oldData)
	{
		//let it first know that it will be removed
		oldData->onBlockRemove(this, position);
	}
	blockContainer::removeBlock(position, oldBlockID, oldData, minimalLoadLevel);
	if (oldBlockID == blockID::portal)
	{
		const auto it = std::find(portalPositions.begin(), portalPositions.end(), position);
		if (it != portalPositions.end())
		{
			portalPositions.erase(it);
		}

		//remove all frames
		std::vector<veci2> relativePositions = std::vector<veci2>({ veci2(-1,0),veci2(1,0) ,veci2(0,-1) ,veci2(0,1) });
		for (cveci2 relativePosition : relativePositions)
		{
			cveci2 absolutePosition = position + relativePosition;
			if (getBlockID(absolutePosition, minimalLoadLevel) == blockID::portal)
			{
				setBlockID(absolutePosition, blockID::air, minimalLoadLevel);
			}
		}
	}
}
bool tickableBlockContainer::cropRelativeLineToContainer(vec2& p0, vec2& p1)
{
	return true;
}
bool tickableBlockContainer::canAddUpdates(cveci2& position)
{
	return inBounds(position);
}
void tickableBlockContainer::setBlockWithData(cveci2& position, const blockID& block, blockData* const& data, const chunkLoadLevel& minimalLoadLevel)
{
	blockContainer::setBlockWithData(position, block, data, minimalLoadLevel);
	if (minimalLoadLevel >= chunkLoadLevel::updateLoaded)
	{
		//BLOCK UPDATES CAN SPREAD
		addBlockUpdatePositionsAround(position);
	}
	addLevelUpdatePosition(position);

}
void tickableBlockContainer::addUpdatePosition(cveci2& position)
{
	if (canAddUpdates(position))
	{
		emittanceUpdateQueue.push_back(position);
		addBlockUpdatePosition(position);
	}
}
std::vector<entity*> tickableBlockContainer::findNearEntities(cvec2& position, cfp& radius)
{
	return rootDimension->findNearEntities(position, containerToRootTransform.multSizeMatrix(cvec2(radius)).maximum());
}
std::vector<entity*> tickableBlockContainer::findNearEntities(crectangle2& searchBox)
{
	return rootDimension->findNearEntities(containerToRootTransform.multRectMatrix(searchBox));
}
bool tickableBlockContainer::findRaycastRecursive(cvec2& absolutePosition, cvec2& absoluteDestination, veci2& resultingBlockPosition, veci2& adjacentBlockPosition, vec2& exactBlockIntersection, tickableBlockContainer*& resultingContainer)
{
	resultingContainer = nullptr;
	mat3x3 rootToContainerTransform = containerToRootTransform.inverse();
	vec2 croppedP0 = rootToContainerTransform.multPointMatrix(absolutePosition), croppedP1 = rootToContainerTransform.multPointMatrix(absoluteDestination);
	tickableBlockContainer* noCollisionBlockContainer = nullptr;

	if (cropRelativeLineToContainer(croppedP0, croppedP1)) {
		fp closestDistanceSquared = INFINITY;
		fp tMin = 0;
		if (findRayCast(croppedP0, croppedP1, resultingBlockPosition, adjacentBlockPosition, exactBlockIntersection,
			[this, &croppedP0, &croppedP1, &tMin](cveci2& position)
			{
				const auto rectsToTest = this->getBlock(position)->getCollisionData(this, position).getCollisions(collisionTypeID::willCollide);
				for (const auto& coll : rectsToTest) {
					fp tMax = 0;
					if (collidedistance2d(croppedP0, croppedP1, coll.hitboxCollidingWith, tMin, tMax)) {
						return true;
					}

				}
				return false;
			}
		)) {
			exactBlockIntersection = croppedP0 + tMin * (croppedP1 - croppedP0).normalized();
			closestDistanceSquared = (exactBlockIntersection - croppedP0).lengthSquared();
			resultingContainer = this;
		}
		else if (this->getBlockID(floorVector(exactBlockIntersection)) != blockID::air)
		{
			noCollisionBlockContainer = this;
		}
		exactBlockIntersection = containerToRootTransform.multPointMatrix(exactBlockIntersection);
		for (const auto child : childContainers)
		{
			veci2 resultingChildBlockPosition;
			veci2 adjacentChildBlockPosition;
			vec2 absoluteChildBlockIntersection;
			tickableBlockContainer* grandChild;
			if (child->findRaycastRecursive(absolutePosition, absoluteDestination, resultingChildBlockPosition, adjacentChildBlockPosition, absoluteChildBlockIntersection, grandChild)) {
				cvec2& relativeFoundPosition = rootToContainerTransform.multPointMatrix(absoluteChildBlockIntersection);
				fp distanceSquared = (relativeFoundPosition - croppedP0).lengthSquared();
				if (distanceSquared < closestDistanceSquared)
				{
					closestDistanceSquared = distanceSquared;
					resultingBlockPosition = resultingChildBlockPosition;
					adjacentBlockPosition = adjacentChildBlockPosition;
					exactBlockIntersection = absoluteChildBlockIntersection;
					resultingContainer = grandChild;
				}
			}
			else if (!resultingContainer && grandChild && grandChild->getBlockID(resultingChildBlockPosition) != blockID::air)
			{
				resultingBlockPosition = resultingChildBlockPosition;
				adjacentBlockPosition = adjacentChildBlockPosition;
				exactBlockIntersection = absoluteChildBlockIntersection;
				noCollisionBlockContainer = grandChild;
			}
		}
		if (resultingContainer)
		{
			return true;
		}
		else if (noCollisionBlockContainer) {
			resultingContainer = noCollisionBlockContainer;
		}
	}
	return false;
}
lightLevel tickableBlockContainer::getVisibleLightLevel(cveci2& pos)
{
	return rootDimension->getVisibleLightLevel(floorVector(containerToRootTransform.multPointMatrix(cvec2(pos))));
}
bool tickableBlockContainer::fillNetherPortal(cveci2& position)
{
	if (rootDimension->identifier == dimensionID::end)
	{
		return false;
	}
	std::vector<veci2> relativeCheckPositions = std::vector<veci2>({ veci2(-1, 0), veci2(1, 0), veci2(0, -1), veci2(0, 1) });
	for (veci2 relativeCheckPosition : relativeCheckPositions)
	{
		veci2 absoluteCheckPosition = position + relativeCheckPosition;
		if (getBlockID(absoluteCheckPosition) == blockID::obsidian)
		{
			//it is worth checking

			int portalDistance[directionCount2D]{};
			//raycast to all sides
			for (size_t portalSideIndex = 0; portalSideIndex < relativeCheckPositions.size(); portalSideIndex++)
			{
				veci2 portalSideDirection = relativeCheckPositions[portalSideIndex];
				for (int distance = 1;; distance++)
				{
					blockID id = getBlockID(position + portalSideDirection * distance);
					if (id != blockID::air)
					{
						if (id == blockID::obsidian)
						{
							//found border of the portal
							portalDistance[portalSideIndex] = distance;
							break;
						}
						else if (id == blockID::air)
						{
							if (distance >= maximumPortalWidth)
							{
								return false;
							}
						}
						else
						{
							//found other block
							return false;
						}
					}
				}
			}
			//found all borders
			cveci2 minPortalBorder = position - cveci2(portalDistance[0], portalDistance[2]);
			cveci2 maxPortalBorder = position + cveci2(portalDistance[1], portalDistance[3]);
			cint portalFrameWidth = maxPortalBorder.x - minPortalBorder.x - 2 + 1;
			cint portalFrameHeight = maxPortalBorder.y - minPortalBorder.y - 2 + 1;
			if (portalFrameWidth < minimumPortalWidth || portalFrameWidth > maximumPortalWidth ||
				portalFrameHeight < minimumPortalHeight || portalFrameHeight > maximumPortalHeight)
			{
				//wrong size
				return false;
			}
			//check if the borders have obsidian
			for (int j = minPortalBorder.y; j <= maxPortalBorder.y; j++)
			{
				for (int i = minPortalBorder.x; i <= maxPortalBorder.x; i++)
				{
					const blockID checkFor = (i == minPortalBorder.x || i == maxPortalBorder.x || j == minPortalBorder.y || j == maxPortalBorder.y) ?
						blockID::obsidian : blockID::air;
					const blockID id = getBlockID(cveci2(i, j));
					if (id != checkFor)
					{
						return false;
					}
				}
			}
			//portal verified
			//fill with portal frames
			setBlockRange(minPortalBorder + cveci2(1), maxPortalBorder + cveci2(-1), blockID::portal, chunkLoadLevel::updateLoaded);

			//add portal to list
			portalPositions.push_back(cveci2((minPortalBorder.x + maxPortalBorder.x) / 2, minPortalBorder.y + 1));
			return true;
		}
	}
	return false;
}
void tickableBlockContainer::setBlockID(cveci2& pos, const blockID& block, const chunkLoadLevel& minimalLoadLevel)
{
	if ((block == blockID::fire) && fillNetherPortal(pos))
	{
		return;
	}
	else
	{
		blockContainer::setBlockID(pos, block, minimalLoadLevel);
	}
}


void tickableBlockContainer::openDoorType(cveci2& position, cbool& open)
{
	blockData* data = getBlockData(position);
	openData* toOpenData = dynamic_cast<openData*>(data);
	if (toOpenData->isOpen != open)
	{
		toOpenData->isOpen = open;

		const blockID& blockChanged = getBlockID(position);
		vec2 soundPosition;
		if (isDoubleBlock(blockChanged))
		{
			const directionID& directionFacing = hasFacingData(blockChanged) ? dynamic_cast<facingData*>(data)->directionFacing : directionID::positiveY;
			cveci2& otherPartAbsoluteLocation = position + getOtherPartRelativeLocation(blockChanged, dynamic_cast<doubleBlockData*>(data)->isPart0, directionFacing);
			openData* dataOtherPart = dynamic_cast<openData*>(getBlockData(otherPartAbsoluteLocation));
			if (dataOtherPart)
			{
				dataOtherPart->isOpen = !dataOtherPart->isOpen;
			}
			soundPosition = ((cvec2(position) + cvec2(otherPartAbsoluteLocation)) * 0.5) + vec2(0.5);
		}
		else
		{
			soundPosition = position + vec2(0.5);
		}
		if (toOpenData->isOpen)
		{
			if (isFenceGate(blockChanged))
			{
				fenceGateOpenSound->playRandomSound(this, soundPosition);
			}
			else if (isTrapDoor(blockChanged))
			{
				if (blockChanged == blockID::iron_trapdoor)
				{
					ironTrapDoorOpenSound->playRandomSound(this, soundPosition);
				}
				else
				{
					woodenTrapDoorOpenSound->playRandomSound(this, soundPosition);
				}
			}
			else
			{
				if (blockChanged == blockID::iron_door)
				{
					ironDoorOpenSound->playRandomSound(this, soundPosition);
				}
				else
				{
					woodenDoorOpenSound->playRandomSound(this, soundPosition);
				}
			}
		}
		else
		{
			if (isFenceGate(blockChanged))
			{
				fenceGateCloseSound->playRandomSound(this, soundPosition);
			}
			else if (isTrapDoor(blockChanged))
			{
				if (blockChanged == blockID::iron_trapdoor)
				{
					ironTrapDoorCloseSound->playRandomSound(this, soundPosition);
				}
				else
				{
					woodenTrapDoorCloseSound->playRandomSound(this, soundPosition);
				}
			}
			else
			{
				if (blockChanged == blockID::iron_door)
				{
					ironDoorCloseSound->playRandomSound(this, soundPosition);
				}
				else
				{
					woodenDoorCloseSound->playRandomSound(this, soundPosition);
				}
			}
		}
	}
}
void tickableBlockContainer::fuseTNT(cveci2& position, cbool& ignitedByExplosion)
{
	setBlockID(position, blockID::air, chunkLoadLevel::updateLoaded);
	tnt* fusedTNT = (tnt*)summonEntity(entityID::tnt, this, cvec2(position) + cvec2(0.5));
	fusedTNT->ticksUntilFuse = ignitedByExplosion ? rand(currentRandom, 10, 30) : 40;
	fusedTNT->speed = cvec2(randFp(-0.02, 0.02), 0.2);
	fuseSound->playRandomSound(this, cvec2(position) + cvec2(0.5));
}

void tickableBlockContainer::createExplosion(cvec2& position, cfp& explosionPower, cbool& startFire)
{
	//play explosion sound
	explosionSound->playRandomSound(this, position);

	//TODO: add particles and explode other things, fire
	constexpr fp explosionRayStepSize = 0.3;
	constexpr fp rayIntensityReductionPerBlock = 0.75;
	constexpr fp rayIntensityReductionPerStep = rayIntensityReductionPerBlock * explosionRayStepSize;

	cfp maximumBlastRadius = 1.3 * (explosionPower / rayIntensityReductionPerStep) * 0.3;

	cint rayCount = (int)(maximumBlastRadius * 0x4);

	//damage entities
	const std::vector<entity*> nearEntities = findNearEntities(position, maximumBlastRadius);

	fastArray<rectangle2> hitboxes = fastArray<rectangle2>((fsize_t)nearEntities.size());
	for (size_t i = 0; i < nearEntities.size(); i++)
	{
		hitboxes[i] = nearEntities[i]->calculateHitBox();
	}

	auto exposureArray = fastArray<fp>((fsize_t)nearEntities.size(), true);

	auto exposureDivisionArray = fastArray<int>((fsize_t)nearEntities.size(), true);
	auto distanceArray = fastArray<fp>((fsize_t)nearEntities.size(), true);

	fastArray<vec2> knockBackArray = fastArray<vec2>((fsize_t)nearEntities.size(), true);

	cfp rayAngleOffset = randFp();

	for (int explosionRayIndex = 0; explosionRayIndex < rayCount; explosionRayIndex++)
	{
		fp rayIntensity = (0.7 + randFp(currentRandom, 0.6)) * explosionPower;
		cfp rayAngle = ((explosionRayIndex + rayAngleOffset) / rayCount) * math::PI2;

		cvec2 explosionRayNormal = vec2::getrotatedvector(rayAngle);
		cvec2 explosionRayStep = explosionRayNormal * explosionRayStepSize;

		vec2 rayPosition = position;
		veci2 blockPositionToBreak = floorVector(rayPosition);
		block* blockToBreak = getBlock(blockPositionToBreak);
		for (int currentRayStep = 0; true; currentRayStep++)
		{
			//cfp reduction = blockToBreak->blastResistance == -1 ?
			//	rayIntensityReductionPerStep :
			//	(blockToBreak->blastResistance + 0.3) * explosionRayStepSize;

			rayIntensity -= rayIntensityReductionPerStep;

			//update block position
			cveci2 newBlockPosition = floorVector(rayPosition);
			if (newBlockPosition != blockPositionToBreak)
			{
				if (blockToBreak->blastResistance >= 0)
				{
					if (blockToBreak->identifier == blockID::tnt)
					{
						fuseTNT(blockPositionToBreak, true);
					}
					else
					{
						//harvest the block
						harvestBlock(dropData(nullptr, nullptr, (int)explosionPower), blockPositionToBreak);

						//break the block
						setBlockID(blockPositionToBreak, (startFire && randChance(currentRandom, 3)) ? blockID::fire : blockID::air, chunkLoadLevel::updateLoaded);
					}
				}
				blockPositionToBreak = newBlockPosition;
				blockToBreak = getBlock(blockPositionToBreak);
			}

			if (rayIntensity <= 0)
			{
				//the ray stopped here
				//check if an entity is hit by it
				if (currentRayStep > 0)
				{
					cfp maximalRayDistance = currentRayStep * explosionRayStepSize;
					for (size_t entityIndex = 0; entityIndex < nearEntities.size(); entityIndex++)
					{
						const entity* const& e = nearEntities[entityIndex];
						fp tmin, tmax;
						crectangle2& hitBox = e->calculateHitBox(e->position);
						//check for intersection
						if (collidedistance2dDirection(position, explosionRayNormal, hitboxes[entityIndex], tmin, tmax))
						{
							exposureDivisionArray[entityIndex]++;
							if (tmin < maximalRayDistance)
							{
								//more rays = less exposure per ray
								//distance 2, raycount 4 : +  1/8 per ray
								exposureArray[entityIndex]++;

								distanceArray[entityIndex] += math::maximum(tmin, (fp)0);
								//add explosion knockback
								knockBackArray[entityIndex] += explosionRayNormal;
							}
						}
					}
				}
				break;
			}

			rayPosition += explosionRayStep;
		}
	}

	cfp damageMultiplier = 7 * explosionPower;//multiply by number to get roughly the same damage as minecraft

	//damage entities based on exposure
	for (size_t entityIndex = 0; entityIndex < nearEntities.size(); entityIndex++)
	{
		//to make sure there are no NAN's
		exposureDivisionArray[entityIndex] = math::maximum(exposureDivisionArray[entityIndex], 1);
		cfp multiplier = 1.0 / exposureDivisionArray[entityIndex];
		exposureArray[entityIndex] *= multiplier;
		cfp averageDistance = distanceArray[entityIndex] * multiplier;
		cfp distancePart = averageDistance / maximumBlastRadius;

		//value of 0 to one
		cfp impact = (1 - distancePart) * exposureArray[entityIndex];
		cvec2 averageKnockBackNormal = knockBackArray[entityIndex] * multiplier;
		cvec2 knockBack = averageKnockBackNormal * (explosionPower * impact) * 0.5;
		cfp damage = ((impact * impact * impact) * damageMultiplier) + 1;
		nearEntities[entityIndex]->addDamageSource(damage, std::make_shared<damageSource>(blastDamage));
		nearEntities[entityIndex]->handleCollision(knockBack, humanWeight);
	}
}

void tickableBlockContainer::tick()
{
	//you CANT erase from this list except here!
	blockUpdatePositions.update();
	for (size_t i = 0; i < blockUpdatePositions.size; i++)
	{
		blockData* data = getBlockData(blockUpdatePositions[i]);
		if (data)
		{
			data->ticked = false;
		}
	}
	for (size_t i = 0; i < blockUpdatePositions.size; i++)
	{
		blockData* data = getBlockData(blockUpdatePositions[i]);
		//remove empty, double and finished updates
		if ((!data) || data->ticked)
		{
			blockUpdatePositions.erase(i);
		}
		else
		{
			if (data->tick(this, blockUpdatePositions[i]))
			{
				data->ticked = true;//only set data->ticked to true when it still expects more ticks. else the block might be deleted
			}
			else
			{
				blockUpdatePositions.erase(i);
			}
		}
	}
}

tickableBlockContainer::~tickableBlockContainer()
{
	if (parentContainer)
	{
		parentContainer->removeChildContainer(this);
	}
}
lightLevel tickableBlockContainer::getMaximumInternalLightLevel(cveci2& pos)
{
	const lightLevel internalSunLight = getInternalSunLightLevel(pos);
	if (internalSunLight == maxLightLevel)
	{
		return maxLightLevel;
	}
	else
	{
		const lightLevel blockLight = getBlockLightLevel(pos);
		return math::maximum(blockLight, internalSunLight);
	}
}

entity* tickableBlockContainer::findUUID(cvec2& position, cfp& range, const uuid& id)
{
	return rootDimension->findUUID(containerToRootTransform.multPointMatrix(position), containerToRootTransform.multSizeMatrix(cvec2(range)).maximum(), id);
}



void tickableBlockContainer::updateChildToParentTransform(cmat3x3& newTransform)
{
	childToParentTransform = newTransform;
	updateContainerToRootTransform();
}

void tickableBlockContainer::updateContainerToRootTransform()
{
	containerToRootTransform = mat3x3::cross(parentContainer->containerToRootTransform, childToParentTransform);
	for (tickableBlockContainer* const& movingBlocks : childContainers)
	{
		movingBlocks->updateContainerToRootTransform();
	}
}

void tickableBlockContainer::updateSpeedInParent(cvec2& newSpeed)
{
	speedInParent = newSpeed;
	updateSpeedInRoot();
}

void tickableBlockContainer::updateSpeedInRoot()
{
	speedInRoot = parentContainer->speedInRoot + speedInParent;
	for (tickableBlockContainer* const& movingBlocks : childContainers)
	{
		movingBlocks->updateSpeedInRoot();
	}
}

void tickableBlockContainer::addChildContainer(tickableBlockContainer* const& movingBlocks, cmat3x3& childToParentTransform)
{
	childContainers.push_back(movingBlocks);
	movingBlocks->parentContainer = this;
	movingBlocks->rootDimension = rootDimension;
	movingBlocks->updateChildToParentTransform(childToParentTransform);
}

void tickableBlockContainer::removeChildContainer(tickableBlockContainer* const& movingBlocks)
{
	childContainers.erase(std::find(childContainers.begin(), childContainers.end(), movingBlocks));
	for (size_t i = 0; i < movingBlocks->childContainers.size(); i++)
	{
		addChildContainer(movingBlocks->childContainers[i], mat3x3::cross(movingBlocks->childToParentTransform, movingBlocks->childContainers[i]->childToParentTransform));
	}
}

void tickableBlockContainer::setParentContainer(tickableBlockContainer* newParentContainer, cmat3x3& newTransform)
{
	if (parentContainer != newParentContainer)
	{
		if (parentContainer)
		{
			parentContainer->removeChildContainer(this);
		}
		if (newParentContainer)
		{
			newParentContainer->addChildContainer(this, newTransform);
		}
	}
	else
	{
		updateChildToParentTransform(newTransform);
	}
}

tickableBlockContainer* tickableBlockContainer::getDeepestNestedContainer(cvec2& relativePosition)
{
	for (const auto c : childContainers)
	{
		if (c->inBounds(floorVector(c->childToParentTransform.inverse().multPointMatrix(relativePosition))))
		{
			return c->getDeepestNestedContainer(relativePosition);
		}
	}
	return this;
}

void tickableBlockContainer::addBlockUpdatePositionsAround(cveci2& pos)
{
	//to prevent block updates loading extra chunks
	if (canAddUpdates(pos))
	{
		//also itself, you never know
		cint relativePositionCount = 5;

		static cveci2 relativePositions[relativePositionCount]
		{
						  cveci2(0,-1),
			cveci2(-1, 0),cveci2(0, 0),cveci2(1, 0),
						  cveci2(0, 1)
		};
		//always add blockupdates, even if this block is not full of water
		for (cveci2& relativePosition : relativePositions)
		{
			cveci2 absolutePosition = relativePosition + pos;
			addBlockUpdatePosition(absolutePosition);
		}
	}
}
void tickableBlockContainer::addBlockUpdatePosition(cveci2& position)
{
	if (canAddUpdates(position))
	{
		blockData* data = getBlockData(position);
		if (data)
		{
			blockUpdatePositions.push_back(position);
		}
	}
}

void tickableBlockContainer::addLevelUpdatePosition(cveci2& position)
{
	if (canAddUpdates(position))
	{
		emittanceUpdateQueue.push_back(position);
	}
}

void tickableBlockContainer::reCalculateLevels()
{
	//copy the list, so the newly added updates won't affect this tick
	std::set<veci2> copiedQueue = std::set<veci2>(emittanceUpdateQueue.begin(), emittanceUpdateQueue.end());
	emittanceUpdateQueue.clear();

	auto lightCompareFunction = [](cveci3& a, cveci3& b)
		{
			return a.y < b.y || (a.y == b.y && a.x < b.x);
		};

	//sort and remove double elements(sorting is not really necessary, it's just for the 'unique' function

	for (int arrayLevelIndex = 0; arrayLevelIndex < (int)levelID::count; arrayLevelIndex++)
	{
		const levelID& currentLevelToCalculate = (levelID)arrayLevelIndex;
		const arrayDataType& currentArrayToCalculate = (arrayDataType)((int)arrayDataType::levelType + arrayLevelIndex);

		//copy the update queue for this data type
		std::set<veci2> updateQueue = std::set<veci2>(copiedQueue.begin(), copiedQueue.end());

		constexpr int checkPositionCount = 0x4;
		constexpr veci2 relativeCheckpositions[checkPositionCount]
		{
			cveci2(-1,0),
			cveci2(1,0),
			cveci2(0,-1),
			cveci2(0,1)
		};
		auto spreadQueue = std::set<veci2>();
		auto decayQueue = std::set<veci3, decltype(lightCompareFunction)>(lightCompareFunction);
		auto glowList = std::vector<veci3>();
		//convert update queue and set light levels
		for (cveci2 it : updateQueue)
		{
			//measure old light level and add it to the decay queue
			cint oldValue = getArrayValue<int>(it, currentArrayToCalculate, chunkLoadLevel::updateLoaded);
			cveci3 decayData = cveci3(it, (int)oldValue);
			decayQueue.insert(decayData);
			//set light level to 0
			setArrayValue<int>(it, 0, currentArrayToCalculate, chunkLoadLevel::worldGenerationLoaded);
		}

		//6. While Q is not empty:
		while (decayQueue.size() > 0)
		{
			auto currentIterator = decayQueue.end();
			--currentIterator;

			cveci3& currentData = *currentIterator;//the current position and the new light level

			const lightLevel oldLight = currentData.z;//the light level spreaded to this place

			cveci2 currentPosition = cveci2(currentData);

			int glowLevel = 0;
			//sunlight
			if ((isLightLevel(currentLevelToCalculate) && ((lightLevelID)((int)currentLevelToCalculate - (int)levelID::light) == lightLevelID::internalSunLight)) && dimensionDataList[rootDimension->identifier]->hasSunLight && currentPosition.y == sunLightHeight)
			{
				glowLevel = maxLightLevel;
			}
			else
			{
				glowLevel = getEmittanceLevel(currentPosition, currentLevelToCalculate);
			}

			if (glowLevel)
			{
				cveci3 glowData = veci3(currentPosition, glowLevel);
				glowList.push_back(glowData);
				spreadQueue.insert(currentPosition);
			}
			//the spread, update, decay and glow queue are sorted

			//we need to look around even if this thing did not spread, so light can spread back
			//this light could have spreaded
			for (int checkPosIndex = 0; checkPosIndex < checkPositionCount; checkPosIndex++)
			{
				veci2 absoluteCheckPosition = currentPosition + relativeCheckpositions[checkPosIndex];
				if (canAddUpdates(absoluteCheckPosition))
				{
					const blockID& spreadedTo = getBlockID(absoluteCheckPosition);
					cint filterStrength = getDecayStrength(currentPosition, absoluteCheckPosition, (levelID)arrayLevelIndex);
					//if this block is updating as well, it will just spread back. else we need to let it spread back
					const lightLevel adjacentLightLevel = getArrayValue<lightLevel>(absoluteCheckPosition, currentArrayToCalculate, chunkLoadLevel::worldGenerationLoaded);

					//the light that spreaded from the old light source
					lightLevel oldSpreadedLight = getSpreadedLightLevel(oldLight, decayStrength[arrayLevelIndex][checkPosIndex], filterStrength);

					if (adjacentLightLevel > oldSpreadedLight)
					{
						//this is wrong if light does not decay; the adjacentlightlevel = oldspreadedlight
						//let the light from the other light source get back.
						insertSortedWithoutDoubles(spreadQueue, absoluteCheckPosition);
					}
					//if oldspreadedlight is zero, that means that the light spread stopped here
					//cant be less than newlight
					else if (oldSpreadedLight > 0 && adjacentLightLevel == oldSpreadedLight)
					{
						//remove all duplicates trying to spread the light because their light level was lower
						auto iterator = spreadQueue.lower_bound(absoluteCheckPosition);
						if (iterator != spreadQueue.end() && *iterator == absoluteCheckPosition)
						{
							spreadQueue.erase(iterator);
						}

						//directly fill with 0 to eliminate duplicates
						setArrayValue<lightLevel>(absoluteCheckPosition, (lightLevel)0, currentArrayToCalculate, chunkLoadLevel::worldGenerationLoaded);
						//if the light levels are the same, that means the light spread reached this point, so lets continue the 'path'
						decayQueue.insert(veci3(absoluteCheckPosition, oldSpreadedLight));
					}
					else
					{
						//this lightlevel is already corrected to avoid duplicates

					}
					//also add a spreadvalue update on the adjacent blocks
					if (needsSpreadValueUpdate(spreadedTo))
					{
						addBlockUpdatePosition(absoluteCheckPosition);
					}
				}
			}
			//erase after usage
			decayQueue.erase(currentIterator);
			//13. Continue looping until Q is exhausted.
		}

		//set all glowing block light levels
		for (cveci3& glowData : glowList)
		{
			setArrayValue<lightLevel>(cveci2(glowData), glowData.z, currentArrayToCalculate, chunkLoadLevel::worldGenerationLoaded);
		}

		//14. Return.
		//6. While Q is not empty:
		while (spreadQueue.size() > 0)
		{
			//7. Set n equal to the first element of Q.
			auto currentIterator = spreadQueue.end();
			--currentIterator;
			cveci2& currentPosition = *currentIterator;
			//get the light level
			const lightLevel newLight = getArrayValue<lightLevel>(currentPosition, currentArrayToCalculate, chunkLoadLevel::worldGenerationLoaded);//the new light level at the place of the block

			for (int checkPosIndex = 0; checkPosIndex < checkPositionCount; checkPosIndex++)
			{
				cveci2& absoluteCheckPosition = currentPosition + relativeCheckpositions[checkPosIndex];
				if (canAddUpdates(absoluteCheckPosition))
				{
					const blockID& spreadTo = getBlockID(absoluteCheckPosition);
					cint filterStrength = getDecayStrength(currentPosition, absoluteCheckPosition, (levelID)arrayLevelIndex);
					//no downwards attenuation
					lightLevel newSpreadedLight = getSpreadedLightLevel(newLight, decayStrength[arrayLevelIndex][checkPosIndex], filterStrength);
					if (newSpreadedLight > 0)
					{
						lightLevel l = getArrayValue<lightLevel>(absoluteCheckPosition, currentArrayToCalculate, chunkLoadLevel::worldGenerationLoaded);
						if (l < newSpreadedLight)
						{
							//directly fill with the spreaded lightlevel to eliminate duplicates
							setArrayValue<lightLevel>(absoluteCheckPosition, newSpreadedLight, currentArrayToCalculate, chunkLoadLevel::worldGenerationLoaded);

							spreadQueue.insert(absoluteCheckPosition);
							//for blocks that check light or redstone levels
						}
					}
					//also add a spreadvalue update on the adjacent blocks
					if (needsSpreadValueUpdate(spreadTo))
					{
						addBlockUpdatePosition(absoluteCheckPosition);
					}
				}
			}
			//erase after usage
			spreadQueue.erase(currentIterator);
			//13. Continue looping until Q is exhausted.
		}
		//14. Return.
	}
}
lightLevel getSpreadedLightLevel(const lightLevel& spreadFrom, const lightLevel& decayStrength, const lightLevel& filterStrength) noexcept
{
	return math::maximum(spreadFrom - decayStrength - filterStrength, 0);
}