#include "fluidData.h"
#include "tickableBlockContainer.h"
#include "idAnalysis.h"
#include "soundList.h"
#include "dropData.h"
#include "nbt/nbtSerializer.h"

constexpr fluidLevel sideWardsWaterFlowSpeed = math::maximum((maxFluidLevel) / ticksPerRealLifeSecond, 1);
constexpr fluidLevel sideWardsLavaFlowSpeed = math::maximum((maxFluidLevel / 2) / ticksPerRealLifeSecond, 1);
constexpr fluidLevel minWaterFlowHeight = 1;
constexpr fluidLevel minLavaFlowHeight = maxFluidLevel / 4;

bool fluidData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	//1 more thing to fix:
	//because it can't check if the water below will go away, water falls will fall like this: water, no water, water etc.
	const blockID fluidType = containerIn->getBlockID(position);
	const fluidLevel sideWardsFlowSpeed = fluidType == blockID::water ? sideWardsWaterFlowSpeed : sideWardsLavaFlowSpeed;
	const fluidLevel minFlowHeight = fluidType == blockID::water ? minWaterFlowHeight : minLavaFlowHeight;

	const fluidLevel oldFluidLevel = currentFluidLevel;
	bool keepUpdating = false;

	blockID otherLiquid = fluidType == blockID::water ? blockID::lava : blockID::water;

	cint flowDirectionCount = 3;
	const fluidLevel maxTransferAmount[flowDirectionCount]
	{
		maxFluidLevel,
		sideWardsFlowSpeed,
		sideWardsFlowSpeed,
	};

	cveci2 relativeCheckPositions[flowDirectionCount]
	{
		cveci2(0,-1),
		cveci2(-1,0),
		cveci2(1,0),
	};

	for (int i = 0; i < flowDirectionCount; i++)
	{

		cveci2 absoluteCheckPosition = position + relativeCheckPositions[i];
		//flow
		//check bottom
		if (containerIn->canAddUpdates(absoluteCheckPosition) && (relativeCheckPositions[i].y == -1 || oldFluidLevel >= minFlowHeight))
		{
			const blockID blockToFlowTo = containerIn->getBlockID(absoluteCheckPosition);

			fluidData* data = hasFluidData(blockToFlowTo) ? ((fluidData*)containerIn->getBlockData(absoluteCheckPosition)) : nullptr;
			if ((fluidType == blockID::water) && isSponge(blockToFlowTo))
			{
				if (blockToFlowTo == blockID::sponge)
				{
					data = (fluidData*)createBlockData(blockID::wet_sponge);
					containerIn->setBlockWithData(absoluteCheckPosition, blockID::wet_sponge, data);
				}

				//the sponge 'soaks up' all surrounding water
				const fluidLevel& transferAmount = math::minimum(currentFluidLevel, maxSpongeFluidLevel - data->currentFluidLevel);
				currentFluidLevel -= transferAmount;
				data->currentFluidLevel += transferAmount;
			}
			else if (blockToFlowTo == otherLiquid)
			{
				extinguishSound->playRandomSound(containerIn, absoluteCheckPosition + cvec2(0.5));
				const fluidData* otherFluidData = ((fluidData*)containerIn->getBlockData(absoluteCheckPosition));
				if (fluidType == blockID::water)
				{
					//water flows into lava
					if (otherFluidData->currentFluidLevel == maxFluidLevel)
					{
						containerIn->setBlockID(absoluteCheckPosition, blockID::obsidian, chunkLoadLevel::updateLoaded);
					}
					else
					{
						containerIn->setBlockID(absoluteCheckPosition, blockID::cobblestone, chunkLoadLevel::updateLoaded);
					}
				}
				else
				{
					//lava flows into water
					if (relativeCheckPositions[i].y < 0)
					{
						containerIn->setBlockID(absoluteCheckPosition, blockID::stone, chunkLoadLevel::updateLoaded);
					}
					else
					{
						if (currentFluidLevel == maxFluidLevel)
						{
							containerIn->setBlockID(position, blockID::obsidian, chunkLoadLevel::updateLoaded);
						}
						else
						{
							containerIn->setBlockID(position, blockID::cobblestone, chunkLoadLevel::updateLoaded);
						}
						return false;
					}
				}
			}
			else if ((fluidType == blockID::water) && isConcretePowder(blockToFlowTo))
			{
				//solidify
				containerIn->setBlockID(absoluteCheckPosition, (blockID)(((int)blockToFlowTo - (int)blockID::concrete_powder) + (int)blockID::concrete), chunkLoadLevel::updateLoaded);
			}
			else
			{
				const fluidLevel otherFluidLevel = data ? data->currentFluidLevel : 0;
				//sideways check
				if (relativeCheckPositions[i].y == 0 ? currentFluidLevel > otherFluidLevel : otherFluidLevel < maxFluidLevel)
				{
					const fluidLevel maximalTransferAmount = math::minimum(currentFluidLevel, math::minimum(relativeCheckPositions[i].y == 0 ? (fluidLevel)((oldFluidLevel - otherFluidLevel) / 2) : oldFluidLevel, maxTransferAmount[i]));

					if (maximalTransferAmount)
					{
						//try to flow downwards
						if (canBeDestroyedByFluids(blockToFlowTo))
						{
							if (blockToFlowTo != blockID::air)
							{
								containerIn->harvestBlock(dropData(), absoluteCheckPosition, chunkLoadLevel::updateLoaded);
							}
							containerIn->setBlockID(absoluteCheckPosition, fluidType, chunkLoadLevel::updateLoaded);
							((fluidData*)containerIn->getBlockData(absoluteCheckPosition))->currentFluidLevel = maximalTransferAmount;
							currentFluidLevel -= maximalTransferAmount;
						}

						else if (data && ((blockToFlowTo == fluidType)))
						{
							cint totalFluid = data->currentFluidLevel + maximalTransferAmount;
							if (totalFluid > maxFluidLevel) //not enough room
							{
								//no check if no Fluid is being tranferred, because it would not change anything
								currentFluidLevel -= maxFluidLevel - data->currentFluidLevel;
								data->currentFluidLevel = maxFluidLevel;
								keepUpdating = true;
							}
							else //emptied
							{
								data->currentFluidLevel = (fluidLevel)totalFluid;
								currentFluidLevel -= maximalTransferAmount;
							}
						}

					}
				}
			}
		}
	}
	//changed, so blocks around may also change
	if (oldFluidLevel > currentFluidLevel)
	{
		if (currentFluidLevel == 0)
		{
			containerIn->setBlockID(position, blockID::air, chunkLoadLevel::updateLoaded);
			return false;//as this block flowed empty, no update is needed. we can't return the normal way, as currentFluidLevel is undefined now.
		}
		else
		{
			containerIn->addBlockUpdatePositionsAround(position);
		}
	}
	return currentFluidLevel > 0 && keepUpdating;
}

fluidLevel fluidData::getFluidLevel(blockContainer* containerIn, cveci2& position, const blockID& fluid)
{
	return containerIn->getBlockID(position) == fluid ? ((fluidData*)containerIn->getBlockData(position))->currentFluidLevel : 0;
}

void fluidData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	fluidLevel serializedFluidLevel = currentFluidLevel;
	s.serializeValue(std::wstring(L"fluid level"), serializedFluidLevel);

	if ((!s.write) && (serializedFluidLevel > 0) && (serializedFluidLevel <= maxFluidLevel))
	{
		currentFluidLevel = serializedFluidLevel;
	}
}