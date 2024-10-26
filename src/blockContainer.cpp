#include "blockContainer.h"
#include "entity.h"
#include "human.h"
#include "idAnalysis.h"
#include "doubleBlockData.h"
#include "facingData.h"
#include "block.h"
#include "doubleBlock.h"
#include "attachmentDirectionData.h"
#include "lastPowerLevelData.h"
#include "furnaceData.h"
#include "buttonData.h"
#include "leverData.h"
#include "repeaterData.h"
#include "comparatorData.h"
#include "redstoneTorchData.h"
#include "pressurePlateData.h"
#include "world.h"
#include "idConverter.h"
#include "pistonData.h"
#include <vector>
#include <math.h>
#include <list>
#include <random>
#include <string>
#include "arrayDataType.h"
#include "blockData.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "collisionData.h"
#include "collisionDataCollection.h"
#include "collisionEdgeData.h"
#include "collisionTypeID.h"
#include "constants.h"
#include "entityID.h"
#include "array/arraynd/arraynd.h"
#include "globalFunctions.h"
#include "math/axis.h"
#include "math/collisions.h"
#include "math/direction.h"
#include "math/mathFunctions.h"
#include "math/random/random.h"
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectn.h"
#include "levelID.h"
#include "lightLevel.h"
#include "lightLevelID.h"
#include "nbt/nbtData.h"
#include "nbt/nbtDataTag.h"
#include "nbt/nbtSerializer.h"
#include "gameRenderData.h"
#include "dimension.h"
#include "renderBrush.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeColor.h"
void blockContainer::setBlockRange(cveci2 &pos0, cveci2 &pos1, const blockID &block, const chunkLoadLevel &minimalLoadLevel)
{
	cveci2 pos00 = cveci2(math::minimum(pos0.x, pos1.x), math::minimum(pos0.y, pos1.y));
	cveci2 pos11 = cveci2(math::maximum(pos0.x, pos1.x), math::maximum(pos0.y, pos1.y));
	for (veci2 pos = pos00; pos.y <= pos11.y; pos.y++)
	{
		for (pos.x = pos00.x; pos.x <= pos11.x; pos.x++)
		{
			setBlockID(pos, block, minimalLoadLevel);
		}
	}
}
bool blockContainer::replaceBlock(cint &x, cint &y, const blockID &block, const std::vector<blockID> &replaceList, const chunkLoadLevel &minimalLoadLevel)
{
	return replaceBlock(cveci2(x, y), block, replaceList, minimalLoadLevel);
}
bool blockContainer::replaceBlock(cveci2 &pos, const blockID &block, const std::vector<blockID> &replaceList, const chunkLoadLevel &minimalLoadLevel)
{
	if (std::find(replaceList.begin(), replaceList.end(), getBlockID(pos, minimalLoadLevel)) != replaceList.end())
	{
		setBlockID(pos, block, minimalLoadLevel);
		return true;
	}
	return false;
}
void blockContainer::replaceBlockRange(cveci2 &pos0, cveci2 &pos1, const blockID &block, const std::vector<blockID> &replaceList, const chunkLoadLevel &minimalLoadLevel)
{
	crectanglei2 &affectedRect = crectanglei2::fromOppositeCorners(pos0, pos1);
	cveci2 &rectPos11 = affectedRect.pos1();
	for (veci2 pos = affectedRect.pos0; pos.y <= rectPos11.y; pos.y++)
	{
		for (pos.x = affectedRect.pos0.x; pos.x <= rectPos11.x; pos.x++)
		{
			replaceBlock(pos, block, replaceList, minimalLoadLevel);
		}
	}
}
bool blockContainer::blockContains(cveci2 &position, const std::vector<blockID> &checkList)
{
	return std::find(checkList.begin(), checkList.end(), getBlockID(position)) != checkList.end();
}
bool blockContainer::blockRangeContains(cveci2 &pos00, cveci2 &pos11, const std::vector<blockID> &checkList)
{
	for (veci2 pos = pos00; pos.y <= pos11.y; pos.y++)
	{
		for (pos.x = pos00.x; pos.x <= pos11.x; pos.x++)
		{
			if (blockContains(pos, checkList))
			{
				return true;
			}
		}
	}
	return false;
}
bool blockContainer::blockRangeContainsOnly(cveci2 &pos00, cveci2 &pos11, const std::vector<blockID> &checkList)
{
	for (veci2 pos = pos00; pos.y <= pos11.y; pos.y++)
	{
		for (pos.x = pos00.x; pos.x <= pos11.x; pos.x++)
		{
			if (std::find(checkList.begin(), checkList.end(), getBlockID(pos)) == checkList.end())
			{
				return false; // one of the items found
			}
		}
	}
	return true;
}

void blockContainer::removeBlock(cveci2 &position, const blockID &oldBlockID, blockData *const &oldData, const chunkLoadLevel &minimalLoadLevel)
{

	// remove the current block before calling setblockid for the other part
	setArrayValue<blockID>(position, blockID::air, arrayDataType::blockIDType, minimalLoadLevel);
	// set to air, not the new block to prevent infinite loops from happening
	if (oldData)
	{

		if (isDoubleBlock(oldBlockID))
		{
			doubleBlockData *oldDoubleBlockData = dynamic_cast<doubleBlockData *>(oldData);

			const directionID oldBlockDirectionFacing = hasFacingData(oldBlockID) ? dynamic_cast<facingData *>(oldData)->directionFacing : standardUpFacingBlockDirection;
			cveci2 otherPos = position + getOtherPartRelativeLocation(oldBlockID, oldDoubleBlockData->isPart0, oldBlockDirectionFacing);

			bool shouldBreakOtherBlock = true;

			// make sure pistons don't break their bottom block when they retract
			if (isPiston(oldBlockID) && isPiston(getBlockID(otherPos)))
			{
				pistonData *part0Data = dynamic_cast<pistonData *>(oldDoubleBlockData->isPart0 ? (oldDoubleBlockData) : getBlockData(otherPos, minimalLoadLevel));
				shouldBreakOtherBlock = part0Data->pushProgress == 1 && (!part0Data->retracting);
			}

			if (shouldBreakOtherBlock)
			{
				if (getBlockID(otherPos, minimalLoadLevel) == oldBlockID)
				{

					// set the other block to air
					setBlockID(otherPos, blockID::air, minimalLoadLevel);
				}
			}
		}

		setBlockData(position, nullptr);
		delete oldData;
		// the block updates will be stopped automatically
	}
}

void blockContainer::setBlockData(cveci2 &position, blockData *const &data, const chunkLoadLevel &minimalLoadLevel)
{
	setArrayValue<blockData *>(position, data, arrayDataType::blockDataType, minimalLoadLevel);
}

void blockContainer::setInternalSunLightLevel(cveci2 &position, lightLevel const &level, const chunkLoadLevel &minimalLoadLevel)
{
	setArrayValue<lightLevel>(position, level, (arrayDataType)((int)arrayDataType::levelType + (int)levelID::light + (int)lightLevelID::internalSunLight), minimalLoadLevel);
}

void blockContainer::setBlockLightLevel(cveci2 &position, lightLevel const &level, const chunkLoadLevel &minimalLoadLevel)
{
	setArrayValue<lightLevel>(position, level, (arrayDataType)((int)arrayDataType::levelType + (int)levelID::light + (int)lightLevelID::blockLight), minimalLoadLevel);
}

void blockContainer::setPowerLevel(cveci2 &position, powerLevel const &level, const chunkLoadLevel &minimalLoadLevel)
{
	setArrayValue<powerLevel>(position, level, (arrayDataType)((int)arrayDataType::levelType + (int)levelID::powerLevel), minimalLoadLevel);
}
void blockContainer::replaceCircleCentered(cvec2 &pos, cvec2 &size, const blockID &block, const std::vector<blockID> &replaceList)
{
	replaceCircle(crectangle2(pos + size * -0.5, size), block, replaceList);
}
void blockContainer::replaceCircle(crectangle2 &rect, const blockID &block, const std::vector<blockID> &replaceList)
{
	cint MinY = (int)ceil(rect.y);
	cint MaxY = (int)ceil(rect.y + rect.h); //+1 for also filling the last pixel
	// crop

	cfp midx = rect.x + rect.w * .5;
	cfp midy = rect.y + rect.h * .5;
	cfp multx = 2.0 / rect.w;
	cfp multy = 2.0 / rect.h;

	for (int j = MinY; j < MaxY; j++)
	{
		// circle equation:
		// x * x + y * y = r * r
		// ellipse equation:
		//(dx * multx)^2 + (dy*multy)^2 = 1
		//(dx * multx)^2 = 1 - (dy * multy) ^2
		// dx * multx = sqrt(1 - (dy * multy) ^ 2)
		// dx = sqrt(1 - (dy * multy) ^2) / multx
		cfp val = 1 - math::squared((midy - j) * multy);
		if (val > 0)
		{
			cfp dx = sqrt(val) / multx;

			// 0.5 to 1.5:
			// only fill two pixels
			cint minX = (int)ceil(midx - dx);
			cint maxX = (int)floor(midx + dx) + 1; //+1 for also filling the last pixel

			for (int i = minX; i < maxX; i++)
			{
				replaceBlock(i, j, block, replaceList);
			}
		}
	}
}

void blockContainer::setBlockWithData(cveci2 &position, const blockID &block, blockData *const &data, const chunkLoadLevel &minimalLoadLevel)
{
	if (inBounds(position))
	{
		const blockID &oldBlock = getBlockID(position);

		// remove old blockdata
		blockData *oldData = getBlockData(position, minimalLoadLevel);

		removeBlock(position, oldBlock, oldData, minimalLoadLevel);

		setArrayValue<blockID>(position, block, arrayDataType::blockIDType, minimalLoadLevel);
		setBlockData(position, data, minimalLoadLevel);
	}
}
void blockContainer::setBlockID(cveci2 &position, const blockID &block, const chunkLoadLevel &minimalLoadLevel)
{
	if (inBounds(position))
	{
		if (isDoubleBlockWhenPlaced(block))
		{
			if (!inBounds(position + ((doubleBlock *)blockList[(int)block])->part1RelativeLocation))
			{
				return;
			}
		}

		setBlockWithData(position, block, createBlockData(block), minimalLoadLevel);
		if (isDoubleBlockWhenPlaced(block))
		{
			// set the other block to this block, even if it is obsidian. the checking part should be done by the player place algorithm
			cveci2 part1ToPart0 = -((doubleBlock *)blockList[block])->part1RelativeLocation;
			cveci2 otherPos = position + ((doubleBlock *)blockList[block])->part1RelativeLocation;
			// it is the bottom block if the bottom block is not the same block.
			// if below it is the same block, we can know that by checking whick part it is
			// if it is placed inside the same block, it will be removed already
			cbool isPart0 = getBlockID(position + part1ToPart0) != block || !(dynamic_cast<doubleBlockData *>(getBlockData(position + part1ToPart0)))->isPart0;
			// first set part 0, so the other block 'knows'
			(dynamic_cast<doubleBlockData *>(getBlockData(position)))->isPart0 = isPart0;
			if (isPart0)
			{
				// then place the other block
				setBlockID(otherPos, block);
			}
		}
	}
}
std::vector<veci2> blockContainer::getBlockPositions(crectanglei2 &rectToSearch, const std::vector<blockID> &blocksToFind)
{
	return getAffectedPositions(rectToSearch, [this, &blocksToFind](cveci2 &pos)
								{ return std::find(blocksToFind.begin(), blocksToFind.end(), getBlockID(pos)) != blocksToFind.end(); });
}

blockID blockContainer::getBlockID(cveci2 &position, const chunkLoadLevel &minimalLoadLevel)
{
	return getArrayValue<blockID>(position, arrayDataType::blockIDType, minimalLoadLevel);
}
blockData *blockContainer::getBlockData(cveci2 &position, const chunkLoadLevel &minimalLoadLevel)
{
	return getArrayValue<blockData *>(position, arrayDataType::blockDataType, minimalLoadLevel);
}
lightLevel blockContainer::getInternalSunLightLevel(cveci2 &position, const chunkLoadLevel &minimalLoadLevel)
{
	return getArrayValue<lightLevel>(position, (arrayDataType)((int)arrayDataType::levelType + (int)levelID::light + (int)lightLevelID::internalSunLight), minimalLoadLevel);
}
lightLevel blockContainer::getBlockLightLevel(cveci2 &position, const chunkLoadLevel &minimalLoadLevel)
{
	return getArrayValue<lightLevel>(position, (arrayDataType)((int)arrayDataType::levelType + (int)levelID::light + (int)lightLevelID::blockLight), minimalLoadLevel);
}

powerLevel blockContainer::getPowerLevel(cveci2 &position, const chunkLoadLevel &minimalLoadLevel)
{
	return getArrayValue<powerLevel>(position, (arrayDataType)((int)arrayDataType::levelType + (int)levelID::powerLevel), minimalLoadLevel);
}
collisionDataCollection blockContainer::getBlockCollisionData(cveci2 &position)
{
	return getBlock(position, chunkLoadLevel::updateLoaded)->getCollisionData(this, position);
}

void blockContainer::addOres(cveci2 &pos, const blockID &b, cint &oreCount, std::mt19937 &randomToUse, const std::vector<blockID> &replaceList)
{
	addOres(pos, b, oreCount, randomToUse, replaceList, std::vector<veci2>(directionVectors2D, directionVectors2D + directionCount2D));
}
void blockContainer::addOres(cveci2 &pos, const blockID &b, cint &oreCount, std::mt19937 &randomToUse, const std::vector<blockID> &replaceList, const std::vector<veci2> &checkPositions)
{
	if (!oreCount)
	{
		return;
	}

	int oresToDeposit = oreCount;
	std::list<veci2> orePositionsToCheck = std::list<veci2>({pos});
	blockID occupyingBlock = getBlockID(pos);
	if (std::find(replaceList.begin(), replaceList.end(), occupyingBlock) == replaceList.end())
	{
		return;
	}
	setBlockID(pos, b);
	oresToDeposit--;
	while (!orePositionsToCheck.empty() && oresToDeposit > 0)
	{
		int index = rand(randomToUse, (int)orePositionsToCheck.size() - 1);
		auto iterator = orePositionsToCheck.begin();
		std::advance(iterator, index);
		veci2 currentPosition = *iterator;

		int expansionPossibilitiesCount = 0;
		int expansionPossibilities[4]{};

		for (int i = 0; i < (int)checkPositions.size(); i++)
		{
			veci2 relativeCheckPos = checkPositions[i];
			// no downwards attenuation
			veci2 absoluteCheckPos = currentPosition + relativeCheckPos;

			if (inBounds(absoluteCheckPos))
			{
				blockID occupyingBlock = getBlockID(absoluteCheckPos);
				if (std::find(replaceList.begin(), replaceList.end(), occupyingBlock) != replaceList.end())
				{
					expansionPossibilities[expansionPossibilitiesCount++] = i;
				}
			}
		}
		if (expansionPossibilitiesCount < 2) // won't be able to expand from this anymore
		{
			orePositionsToCheck.erase(iterator);
		}
		if (expansionPossibilitiesCount)
		{
			// randomly select one from the valid positions and expand from there
			veci2 expansionPosition = currentPosition + checkPositions[(size_t)expansionPossibilities[rand(randomToUse, expansionPossibilitiesCount - 1)]];
			setBlockID(expansionPosition, b);
			oresToDeposit--;
			orePositionsToCheck.push_back(expansionPosition);
		}
	}
}

block *blockContainer::getBlock(cveci2 &pos, const chunkLoadLevel &minimalLoadLevel)
{
	return blockList[(int)getBlockID(pos, minimalLoadLevel)];
}

bool blockContainer::connectionBetween(cvect2<veci2> &positions)
{
	// check both ways
	for (size_t i = 0; i < 2; i++)
	{
		csize_t &otherIndex = 1 - i;
		block *currentBlock = getBlock(positions[i]);
		if (currentBlock->canReplaceBlock)
		{
			return false;
		}
		if (isFallingBlock(currentBlock->identifier))
		{
			return false;
		}

		cbool &attached0 = hasAttachmentDirectionData(currentBlock->identifier);
		if (attached0)
		{
			const blockData *const &currentData = getBlockData(positions[i]);
			cveci2 attachmentDirection = directionVectors2D[(size_t) dynamic_cast<const attachmentDirectionData *>(currentData)->attachmentDirection];
			if (attachmentDirection != (positions[otherIndex] - positions[i]))
			{
				return false;
			}
		}
	}
	return true;
}
int blockContainer::getEmittanceLevel(cveci2 &position, const levelID &arrayLevelDataTypeIndex)
{
	const blockID &b = getBlockID(position);

	if (isLightLevel(arrayLevelDataTypeIndex))
	{
		if ((lightLevelID)((int)arrayLevelDataTypeIndex - (int)levelID::light) == lightLevelID::blockLight)
		{
			if (b == blockID::redstone_lamp)
			{
				return (int)((dynamic_cast<lastPowerLevelData *>(getBlockData(position))->lastPowerLevel / (fp)maxPowerLevel) * brightLightLevel);
			}
			else if (isFurnace(b))
			{
				furnaceData *data = dynamic_cast<furnaceData *>(getBlockData(position));
				return (data->ticksFuelBurned < data->ticksFuelToBurn) ? brightLightLevel : 0;
			}
		}
	}
	else if (arrayLevelDataTypeIndex == levelID::powerLevel)
	{
		if (isButton(b))
		{
			return ((dynamic_cast<buttonData *>(getBlockData(position)))->ticksToPress > 0) ? maxPowerLevel : 0;
		}
		else if (b == blockID::lever)
		{
			return (dynamic_cast<leverData *>(getBlockData(position)))->active ? maxPowerLevel : 0;
		}
		else if (b == blockID::repeater)
		{
			repeaterData *data = dynamic_cast<repeaterData *>(getBlockData(position));
			return ((data->lastPowerLevelBelow == 0) ? data->delayedPowerLevels[delayArray[data->delayArrayIndex] - 1] : data->lockedPowerLevel) > 0 ? maxPowerLevel : 0;
		}
		else if (b == blockID::comparator)
		{
			comparatorData *data = dynamic_cast<comparatorData *>(getBlockData(position));
			if (data->substractionMode)
			{
				return math::maximum(data->lastPowerLevelBehind - data->lastPowerLevelBelow, 0);
			}
			else
			{
				if (data->lastPowerLevelBelow <= data->lastPowerLevelBehind)
				{
					return data->lastPowerLevelBehind;
				}
				else
				{
					return 0;
				}
			}
		}
		else if (b == blockID::redstone_torch)
		{
			redstoneTorchData *data = dynamic_cast<redstoneTorchData *>(getBlockData(position));
			return (data->lastAttachedBlockPowerLevel == 0) ? maxPowerLevel : 0;
		}
		else if (isPressurePlate(b))
		{
			pressurePlateData *data = (pressurePlateData *)(getBlockData(position));
			if (data->lastUpdateTick == currentWorld->ticksSinceStart)
			{
				if (b == blockID::heavy_pressure_plate)
				{
					return math::minimum((powerLevel)data->entitiesStandingOn.size(), maxPowerLevel);
				}
				else
				{
					return ((powerLevel)data->entitiesStandingOn.size() > 0) ? maxPowerLevel : 0;
				}
			}
			else
			{
				return 0;
			}
		}
	}
	return blockList[(int)b]->emittanceLevel[(size_t)arrayLevelDataTypeIndex];
}

int blockContainer::getDecayStrengthExitingFrom(cveci2 &positionFrom, cveci2 &positionTo, const levelID &arrayLevelDataTypeIndex)
{
	const arrayDataType currentDataType = (arrayDataType)((int)arrayDataType::levelType + (int)arrayLevelDataTypeIndex);
	cint &maxStrength = maxArrayValue[(int)currentDataType];

	const blockID &blockFrom = getBlockID(positionFrom);

	if (arrayLevelDataTypeIndex == levelID::powerLevel)
	{
		if ((blockFrom == blockID::repeater) || (blockFrom == blockID::comparator))
		{
			const directionID &direction = dynamic_cast<facingData *>(getBlockData(positionFrom))->directionFacing;
			if ((positionTo - positionFrom) != directionVectors2D[(int)direction])
			{
				return maxStrength;
			}
		}
		else if (blockFrom == blockID::lever)
		{
			const directionID &attachmentDirection = dynamic_cast<leverData *>(getBlockData(positionFrom))->attachmentDirection;

			if ((positionTo - positionFrom) != directionVectors2D[(int)attachmentDirection])
			{
				return maxStrength;
			}
		}
		else if (blockFrom == blockID::redstone_torch)
		{
			if (positionTo == (positionFrom + directionVectors2D[(int)dynamic_cast<attachmentDirectionData *>(getBlockData(positionFrom))->attachmentDirection]))
			{
				return maxStrength;
			}
		}
		else if (blockFrom == blockID::tnt)
		{
			// don't let it blow up all at once
			return maxStrength;
		}
	}
	return 0;
}

int blockContainer::getDecayStrength(cveci2 &positionFrom, cveci2 &positionTo, const levelID &arrayLevelDataTypeIndex)
{
	const block &blockTo = *getBlock(positionTo);
	const arrayDataType currentDataType = (arrayDataType)((int)arrayDataType::levelType + (int)arrayLevelDataTypeIndex);
	cint &maxStrength = maxArrayValue[(int)currentDataType];

	cint toFilterStrength = blockTo.filterStrength[(size_t)arrayLevelDataTypeIndex];
	if (toFilterStrength == maxStrength)
	{
		return maxStrength;
	}
	else
	{
		cint decayStrengthExitingFrom = getDecayStrengthExitingFrom(positionFrom, positionTo, arrayLevelDataTypeIndex);

		return math::minimum(toFilterStrength + decayStrengthExitingFrom, maxStrength);
	}
}
void renderCollisionData(blockContainer *containerIn, cveci2 &position, const resolutionTexture &tex, const gameRenderData &targetData)
{
	renderCollisionData(containerIn, position, crectangle2(tex.getClientRect()), tex, targetData);
}
void renderCollisionData(blockContainer *containerIn, cveci2 &position, const gameRenderData &targetData)
{
	const block *blockToRender = containerIn->getBlock(position);
	renderCollisionData(containerIn, position, *blockToRender->tex, targetData);
}
template <typename brush0Type>
void renderCollisionData(blockContainer *containerIn, cveci2 &position, crectangle2 &brushRect, const brush0Type &b, const gameRenderData &targetData)
{
	const block *blockToRender = containerIn->getBlock(position);
	const mat3x3 &transform = block::getBrushRectToWorldTransform(brushRect, targetData.worldToRenderTargetTransform, position);
	if (blockToRender->blockCollisionType != collisionTypeID::willNotCollide)
	{
		const collisionDataCollection data = blockToRender->getCollisionData(containerIn, position);
		for (const collisionData &rectToRender : data.hitboxes)
		{
			crectangle2 &croppedRect = crectangle2(cvec2(position), cvec2(1)).cropClientRectUnsafe(rectToRender.hitboxCollidingWith);
			crectangle2 &textureRect = getAbsoluteRect(brushRect,
													   crectangle2(croppedRect.pos0 - position, croppedRect.size));

			// crectangle2& drawRect = targetData.worldToRenderTargetTransform.multRectMatrix(croppedRect);
			fillTransparentRectangle(textureRect, transform, b, targetData.renderTarget);
		}
	}
}
bool blockContainer::fitExpandingHitbox(crectangle2 &relativeHitbox, cvec2 &positionToExpandFrom, vec2 &hitboxPosition)
{
	crectangle2 checkArea = crectangle2(positionToExpandFrom - relativeHitbox.size, relativeHitbox.size * 2);
	return fitExpandingHitbox(relativeHitbox, checkArea, positionToExpandFrom, hitboxPosition);
}
bool blockContainer::fitExpandingHitbox(crectangle2 &relativeHitbox, crectangle2 &checkArea, cvec2 &positionToExpandFrom, vec2 &hitboxPosition)
{
	// this function divides the checkArea in rectangles: it 'splices' the checkArea on every collision with an object.
	// when the pieces are so small that nothing collides anymore, we choose the closest position to the expansion point.
	const collisionDataCollection &collisionCollection = getHitboxCollisionData(checkArea, cvec2());
	std::list<rectangle2> slices = std::list<rectangle2>({checkArea});
	std::vector<vec2> possiblePositions = std::vector<vec2>();
	while (slices.size())
	{
		crectangle2 &currentSlice = *slices.begin();
		slices.pop_front();

		// check if the relativeHitbox would fit in here
		if (currentSlice.size.x >= relativeHitbox.size.x && currentSlice.size.y >= relativeHitbox.size.y)
		{
			for (const collisionData &collision : collisionCollection.hitboxes)
			{
				if (collision.type == collisionTypeID::willCollide)
				{
					if (collides2d(currentSlice, collision.hitboxCollidingWith))
					{
						//the checkhitbox intersects with the current slice of checkArea
						for (int axis = 0; axis < 2; axis++)
						{
							// cut at both axes
							//-
							//space left at the - side
							if (collision.hitboxCollidingWith.pos0[axis] > currentSlice.pos0[axis])
							{
								vec2 newSize = currentSlice.size;
								newSize[axis] = collision.hitboxCollidingWith.pos0[axis] - currentSlice.pos0[axis];
								slices.push_back(crectangle2(currentSlice.pos0, newSize));
							}
							//+
							//space left at the + side
							if (collision.hitboxCollidingWith.pos1()[axis] < currentSlice.pos1()[axis])
							{
								rectangle2 newRect = currentSlice;
								newRect.pos0[axis] = collision.hitboxCollidingWith.pos1()[axis];
								newRect.size[axis] = currentSlice.pos1()[axis] - newRect.pos0[axis];
								slices.push_back(newRect);
							}
						}
						// this rectangle is cut already, the pieces can cut themselves
						goto spliced;
					}
				}
			}
			cvec2 desiredCenter = positionToExpandFrom + relativeHitbox.pos0;
			vec2 rectPos00;
			// return the nearest position to 'positionToExpandfrom'
			for (int axis = 0; axis < 2; axis++)
			{
				//+
				if (currentSlice.pos0[axis] > desiredCenter[axis])
				{
					rectPos00[axis] = currentSlice.pos0[axis];
				}
				//-
				else if (currentSlice.pos1()[axis] < desiredCenter[axis] + relativeHitbox.size[axis])
				{
					rectPos00[axis] = currentSlice.pos1()[axis] - relativeHitbox.size[axis];
				}
				else
				{
					rectPos00[axis] = desiredCenter[axis];
				}
			}
			possiblePositions.push_back(rectPos00 - relativeHitbox.pos0);
		}
	spliced:;
	}
	// all possibilities checked, none left
	if (possiblePositions.size())
	{
		vec2 nearestPosition = vec2();
		fp nearestDistanceSquared = INFINITY;
		for (cvec2 &vec : possiblePositions)
		{
			cfp distanceSquared = (vec - positionToExpandFrom).lengthSquared();
			if (distanceSquared < nearestDistanceSquared)
			{
				nearestDistanceSquared = distanceSquared;
				nearestPosition = vec;
			}
		}

		hitboxPosition = nearestPosition;
		return true;
	}
	else
	{
		return false;
	}
}
void blockContainer::moveTileToContainer(cveci2 &sourcePosition, blockContainer &destinationContainer, cveci2 &destinationPosition)
{
	// not removing the block directly after copy, because a double block will be wholly removed
	// not calling onblockremove to avoid deletion
	destinationContainer.setArrayValue<blockID>(destinationPosition, getArrayValue<blockID>(sourcePosition, arrayDataType::blockIDType, chunkLoadLevel::worldGenerationLoaded), arrayDataType::blockIDType, chunkLoadLevel::worldGenerationLoaded);
	destinationContainer.setArrayValue<blockData *>(destinationPosition, getArrayValue<blockData *>(sourcePosition, arrayDataType::blockDataType, chunkLoadLevel::worldGenerationLoaded), arrayDataType::blockDataType, chunkLoadLevel::worldGenerationLoaded);
	destinationContainer.setArrayValue<powerLevel>(destinationPosition, getArrayValue<powerLevel>(sourcePosition, (arrayDataType)((int)arrayDataType::levelType + (int)levelID::powerLevel), chunkLoadLevel::worldGenerationLoaded), (arrayDataType)((int)arrayDataType::levelType + (int)levelID::powerLevel), chunkLoadLevel::worldGenerationLoaded);

	setArrayValue<blockID>(sourcePosition, blockID::air, arrayDataType::blockIDType, chunkLoadLevel::worldGenerationLoaded);
	setArrayValue<blockData *>(sourcePosition, nullptr, arrayDataType::blockDataType, chunkLoadLevel::worldGenerationLoaded);
	setArrayValue<powerLevel>(sourcePosition, 0, (arrayDataType)((int)arrayDataType::levelType + (int)levelID::powerLevel), chunkLoadLevel::worldGenerationLoaded);
}
void blockContainer::addPool(cveci2 &pos, const blockID &block, cint &maxPoolSize)
{
	addPool(pos, block, maxPoolSize, [this](cveci2 &pos, cveci2 &relativeCheckDirection)
			{ return getBlockID(pos) == blockID::air; });
}
void blockContainer::addFluidPool(cveci2 &pos, const blockID &block, cint &maxPoolSize)
{
	addPool(pos, block, maxPoolSize, [this, &block](cveci2 &pos, cveci2 &relativeCheckDirection)
			{
		const blockID& b = getBlockID(pos);
		return canBeDestroyedByFluids(b) || (isFluid(b) && b != block); });
}
collisionDataCollection blockContainer::getHitboxCollisionData(crectangle2 &box)
{
	// check collision with chunk border
	collisionDataCollection dataArray = collisionDataCollection();

	if ((box.size.x > 0) && (box.size.y > 0))
	{
		cint fromX = (int)floor(box.x);
		cint fromY = (int)floor(box.y);
		cvec2 topRight = box.pos1();
		cint toX = (int)floor(topRight.x);
		cint toY = (int)floor(topRight.y);

		// check hitbox
		veci2 checkPos = cveci2();
		for (checkPos.y = fromY; checkPos.y <= toY; checkPos.y++)
		{
			for (checkPos.x = fromX; checkPos.x <= toX; checkPos.x++)
			{
				blockID identifier = getBlockID(checkPos);
				block *data = blockList[(int)identifier];
				collisionTypeID blockCollisionType = data->blockCollisionType;
				if (blockCollisionType != collisionTypeID::willNotCollide)
				{
					const collisionDataCollection &collision = data->getCollisionData(this, checkPos);
					dataArray.addCollisionData(collision);
				}
			}
		}
	}
	return dataArray;
}
collisionDataCollection blockContainer::getHitboxCollisionData(crectangle2 &box, cvec2 &hitboxTranslate)
{
	return getHitboxCollisionData(getBoxContaining(box, hitboxTranslate).expanded(collisionCheckMargin));
}
collisionTypeID blockContainer::getHitboxCollisionType(crectangle2 &box)
{
	collisionDataCollection data = getHitboxCollisionData(box, cvec2());
	data.evaluate(box, cvec2(), 0);
	return data.getMaximumCollision();
}
bool blockContainer::hasFullEdge(cveci2 &position, const directionID &edgeToCheck)
{
	const collisionDataCollection &collection = getBlockCollisionData(position);

	//cveci2 &adjacentPosition = position + directionVectors2D[(int)edgeToCheck];

	cvec2 relativeEdgePosition = cvec2(0.5) + cvec2(directionVectors2D[(int)edgeToCheck]) * 0.5;

	caxisID axis = getAxis(edgeToCheck);

	const collisionEdgeData requiredEdge = collisionEdgeData({cvec2(position[1 - (int)axis], 1)});
	const collisionEdgeData &edgeData = collection.getEdges(position[axis] + relativeEdgePosition[axis], flipDirection(edgeToCheck));

	return requiredEdge.substractCoveringEdges(edgeData).edges.size() == 0;
}
bool blockContainer::canAttachTo(cveci2 &attachmentPosition, const directionID &edgeToCheck)
{
	return (getBlockID(attachmentPosition) == blockID::structure_void) || hasFullEdge(attachmentPosition, edgeToCheck);
}
bool blockContainer::canConnect(cveci2 &from, cveci2 &relativeCheckPosition)
{
	cveci2 to = from + relativeCheckPosition;
	const block *connectionBlock = getBlock(to);
	if (isConnectingBlock(connectionBlock->identifier))
	{
		return true;
	}
	else
	{
		// check if the hitbox of the block allows connecting
		collisionDataCollection otherBlockData = connectionBlock->getCollisionData(this, to);

		collisionEdgeData toEdgeData;
		if (relativeCheckPosition.x == 1)
		{
			toEdgeData = otherBlockData.getEdges(to.x, directionID::positiveX);
		}
		else
		{
			toEdgeData = otherBlockData.getEdges(from.x, directionID::negativeX);
		}

		collisionEdgeData fromEdgeData = collisionEdgeData();
		const blockID connectingBlock = getBlockID(from);
		if (isFence(connectingBlock) || isFenceGate(connectingBlock))
		{
			fromEdgeData.addEdge(from.y + fenceConnectionPoleY[0], fenceConnectionPoleWidth);
			fromEdgeData.addEdge(from.y + fenceConnectionPoleY[1], fenceConnectionPoleWidth);
		}
		else if (isWall(connectingBlock))
		{
			fromEdgeData.addEdge(from.y, wallConnectionHeight);
		}
		else if (connectingBlock == blockID::iron_bars)
		{
			fromEdgeData.addEdge(from.y, 1);
		}

		collisionEdgeData substracted = fromEdgeData.substractCoveringEdges(toEdgeData);
		return substracted.edges.size() == 0;
	}
	return false;
}

vecb2 blockContainer::railTopConnection(cveci2 &pos)
{
	vecb2 topConnection;
	for (int i = 0; i < fenceConnectionPossibilityCount; i++)
	{
		if (isRail(getBlockID(pos + fenceConnectionCheckPositions[i])))
		{
			topConnection[i] = false;
			continue;
		}
		else
		{
			topConnection[i] = isRail(getBlockID(pos + fenceConnectionCheckPositions[i] + cveci2(0, 1)));
		}
	}

	// can't be both a top connection
	return (topConnection.x && topConnection.y) ? vecb2() : topConnection;
}

void serializeBlocks(nbtSerializer &s, const array2d<blockID> &blockIDArray, const array2d<blockData *> &blockDataArray, const array2d<powerLevel> &powerLevelArray)
{
	if (s.serializeArray<int>(std::wstring(L"blocks"), (int *)blockIDArray.baseArray, (int)blockIDArray.size.volume()) && s.converter)
	{
		s.converter->convertArray((blockID *&)blockIDArray.baseArray, blockIDArray.size.volume(), s.converter->itemIDConverter);
	}
	s.serializeArray(std::wstring(L"power levels"), powerLevelArray.baseArray, (int)powerLevelArray.size.volume());
	if (s.push<nbtDataTag::tagList>(std::wstring(L"block data array")))
	{
		if (s.write)
		{
			auto it = blockDataArray.begin();
			for (fsize_t relativeY = 0; relativeY < blockDataArray.size.y; relativeY++)
			{
				for (fsize_t relativeX = 0; relativeX < blockDataArray.size.x; relativeX++, it++)
				{
					if (*it)
					{
						if (s.push<nbtDataTag::tagCompound>())
						{
							veci2 position = veci2(relativeX, relativeY);
							serializeNBTValue(s, std::wstring(L"position"), position);

							(*it)->serializeValue(s);
							s.pop();
						}
					}
				}
			}
		}
		else
		{
			// create block data for all blocks
			// necessary so files converted to newer versions with more block data have block data generated
			for (fsize_t i = 0; i < blockDataArray.size.volume(); i++)
			{
				blockDataArray.baseArray[i] = createBlockData(blockIDArray.baseArray[i]);
			}

			// block data
			// count amount of data
			const std::vector<nbtData *> serializedBlockDataList = s.getChildren();

			for (nbtData *data : serializedBlockDataList)
			{
				if (s.push(data))
				{
					veci2 position;
					serializeNBTValue(s, std::wstring(L"position"), position);
					blockData *data = blockDataArray.getValue(position);

					if (data)
					{
						data->serializeValue(s);
					}
					s.pop();
				}
			}
		}
		s.pop();
	}
}
