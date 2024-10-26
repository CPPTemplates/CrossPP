#include "pistonData.h"
#include "tickableBlockContainer.h"
#include "piston.h"
#include "movableBlockContainer.h"
#include "soundList.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "constants.h"
#include "doubleBlockData.h"
#include "entity.h"
#include "entityID.h"
#include "facingData.h"
#include "globalFunctions.h"
#include "math/direction.h"
#include "math/mathFunctions.h"
#include "math/mattnxn.h"
#include "math/uuid.h"
#include "math/vector/vectn.h"
#include "levelID.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeUUID.h"
bool pistonData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (isPart0)
	{
		const blockID& blockType = containerIn->getBlockID(position);
		cveci2& directionNormal = directionVectors2D[(size_t)directionFacing];
		cveci2& positionBehind = position - directionNormal;
		lastPowerLevelBehind = containerIn->getPowerLevel(positionBehind);

		lastPowerLevelBehind = math::maximum(lastPowerLevelBehind - containerIn->getDecayStrengthExitingFrom(positionBehind, position, levelID::powerLevel), 0);

		cfp& pushDistancePerTick = getPushDistancePerTick();
		piston* summonedPiston = nullptr;
		if ((!blocksMoving) && (((pushProgress == 0) && (pushDistancePerTick > 0)) || ((pushProgress == maxPushProgress) && (pushDistancePerTick < 0))))
		{
			veci2 pistonHeadPosition = position;
			if (pushProgress == maxPushProgress)
			{
				pistonHeadPosition += directionNormal;
			}

			veci2 min = pistonHeadPosition, max = pistonHeadPosition;

			cbool shouldPushOrPull = (pushProgress == 0) || (blockType == blockID::sticky_piston);

			std::set<veci2> pushPositions;
			if (shouldPushOrPull)
			{
				cveci2& blockNextToPistonHeadPosition = ((pushProgress == 0) ? position : (position + directionNormal)) + directionNormal;

				cveci2& pushDirection = (pushProgress == 0) ? directionNormal : (-directionNormal);

				//check if can push
				if (containerIn->getAffectedPositions(blockNextToPistonHeadPosition, directionNormal, pistonPushLimit,
					[containerIn, &position, &pistonHeadPosition, &pushDirection](cveci2& pos, cveci2& checkDirection) {
						if ((pos == position) || (pos == pistonHeadPosition))
						{
							return false;
						}

						if (containerIn->getBlock(pos)->canReplaceBlock)
						{
							return false;
						}

						if (containerIn->connectionBetween({ pos - checkDirection, pos }))
						{
							return true;
						}
						else
						{
							return checkDirection == pushDirection;
						}
					}, pushPositions, std::vector<veci2>(directionVectors2D, directionVectors2D + directionCount2D)))
				{

					for (cveci2& pos : pushPositions)
					{
						if (pushProgress == 0)
						{
							cveci2& replacePos = pos + pushDirection;
							if ((replacePos == position) || (replacePos == pistonHeadPosition))
							{
								return false;
							}
						}
					}

					for (const auto& it : pushPositions)
					{
						min.x = math::minimum(min.x, it.x);
						min.y = math::minimum(min.y, it.y);
						max.x = math::maximum(max.x, it.x);
						max.y = math::maximum(max.y, it.y);
					}
				}
				else
				{
					//too much to push or pull
					return false;
				}
			}

			cvec2& exactPistonHeadPosition = getExactPistonHeadPosition(containerIn, position);
			entity* summonedEntity = summonEntity(entityID::piston, containerIn, exactPistonHeadPosition);
			summonedPiston = (piston*)summonedEntity;

			blocksMoving = summonedPiston->identifier;

			summonedPiston->movingBlocks = new movableBlockContainer((max - min) + 1);
			summonedPiston->movingBlocks->relativeAttachmentPosition = pistonHeadPosition - min;
			containerIn->addChildContainer(summonedPiston->movingBlocks, cmat3x3::translate(exactPistonHeadPosition - summonedPiston->movingBlocks->relativeAttachmentPosition));

			summonedPiston->movingBlocks->setBlockWithData(pistonHeadPosition - min, blockType, new pistonData(directionFacing, false, lastPowerLevelBehind), chunkLoadLevel::updateLoaded);
			if (pushProgress == maxPushProgress)
			{
				retracting = true;
				containerIn->setBlockID(pistonHeadPosition, blockID::air, chunkLoadLevel::updateLoaded);
				retracting = false;
			}

			if (shouldPushOrPull)
			{
				//flood fill with push positions
				for (const auto& it : pushPositions)
				{
					cveci2& relativePosition = it - min;
					containerIn->moveTileToContainer(it, *summonedPiston->movingBlocks, relativePosition);
					containerIn->addUpdatePosition(it);
					summonedPiston->movingBlocks->addUpdatePosition(relativePosition);
				}
			}

			cvec2 exactSoundPosition = position + cvec2(0.5) + (cvec2(directionNormal) * 0.5);
			if (pushProgress == 0)
			{
				pistonExtendSound->playRandomSound(containerIn, exactSoundPosition);
			}
			else
			{
				pistonRetractSound->playRandomSound(containerIn, exactSoundPosition);
			}
		}
		else if (blocksMoving)
		{
			pushProgress = getNextPushProgress(pushDistancePerTick);
			cvec2& exactPistonHeadPosition = getExactPistonHeadPosition(containerIn, position);
			entity* summonedEntity = containerIn->findUUID(exactPistonHeadPosition, 1 + mobSizeMargin, blocksMoving);
			summonedPiston = (piston*)summonedEntity;

			//update variables
			summonedPiston->newPosition = containerIn->containerToRootTransform.multPointMatrix(exactPistonHeadPosition);

			summonedPiston->movingBlocks->setParentContainer(containerIn, cmat3x3::translate(exactPistonHeadPosition - summonedPiston->movingBlocks->relativeAttachmentPosition));

			//so entities don't get pushed further and get stuck in blocks


			if ((pushProgress == 0) || (pushProgress == maxPushProgress))
			{
				if (blocksMoving)
				{
					dropBlocksMoving(containerIn, position);
					return false;
				}
			}
		}
		else
		{
			return false;
		}
		summonedPiston->movingBlocks->updateSpeedInParent(cvec2(directionNormal) * (getNextPushProgress(pushDistancePerTick) - pushProgress) * ticksPerRealLifeSecond);

		//continue pushing or pulling next time
		return true;
	}
	return false;
}

void pistonData::serializeValue(nbtSerializer& s)
{
	facingData::serializeValue(s);
	doubleBlockData::serializeValue(s);
	s.serializeValue(std::wstring(L"last power level behind"), lastPowerLevelBehind);
	s.serializeValue(std::wstring(L"push progress"), pushProgress);
	serializeNBTValue(s, std::wstring(L"blocks moving"), blocksMoving);
}
void pistonData::onBlockRemove(tickableBlockContainer* containerIn, cveci2& position)
{
	if (blocksMoving)
	{
		dropBlocksMoving(containerIn, position);
	}
}
fp pistonData::getPushDistancePerTick() const
{
	return  math::mapValue(getPowerStrength(lastPowerLevelBehind), powerTreshold, (fp)1, -maxPushSpeed, maxPushSpeed);
}
void pistonData::dropBlocksMoving(tickableBlockContainer* containerIn, cveci2& position)
{
	cveci2 directionNormal = directionVectors2D[(int)directionFacing];

	veci2 pistonHeadPosition = position;
	if (pushProgress > (maxPushProgress / 2))
	{
		pistonHeadPosition += directionNormal;
	}
	vec2 exactPistonHeadPosition = getExactPistonHeadPosition(containerIn, position);

	entity* summonedEntity = containerIn->findUUID(exactPistonHeadPosition, 1 + mobSizeMargin, blocksMoving);
	piston* summonedPiston = (piston*)summonedEntity;

	cveci2& add = pistonHeadPosition - summonedPiston->movingBlocks->relativeAttachmentPosition;

	cveci2& pos00 = summonedPiston->movingBlocks->arrayPos00Offset;
	cveci2& pos11 = pos00 + cveci2(summonedPiston->movingBlocks->blockIDArray.size);
	for (int j = pos00.y; j < pos11.y; j++)
	{
		for (int i = pos00.x; i < pos11.x; i++)
		{
			cveci2& relativePosition = cveci2(i, j);
			cveci2& absolutePosition = add + relativePosition;
			//don't overwrite the own piston
			if (summonedPiston->movingBlocks->getBlockID(relativePosition) != blockID::air)
			{
				if (absolutePosition != position)
				{
					summonedPiston->movingBlocks->moveTileToContainer(relativePosition, *containerIn, absolutePosition);
					containerIn->addUpdatePosition(absolutePosition);
				}
			}
		}
	}

	summonedPiston->despawn = true;
	blocksMoving = uuid();
}
vec2 pistonData::getExactPistonHeadPosition(tickableBlockContainer* containerIn, cveci2& position) const
{
	cveci2 directionNormal = directionVectors2D[(int)directionFacing];
	vec2 exactPistonHeadContainerPosition = cvec2(position) + (cvec2(directionNormal) * ((pushProgress / (fp)maxPushProgress)));

	return exactPistonHeadContainerPosition;
}
fp pistonData::getNextPushProgress(cfp& pushSpeed) const
{
	return math::clamp(pushProgress + pushSpeed, (fp)0, maxPushProgress);
}