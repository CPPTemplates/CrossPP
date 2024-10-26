#include "block.h"
#include "fireData.h"
#include "tickableBlockContainer.h"
#include "math/random/random.h"
constexpr int maxFireAge = 0xf;
void fireData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (age < maxFireAge)
	{
		veci2 spreadPosition = cveci2();
		cint checkPositionCount = 0x4;
		cveci2 relativeCheckpositions[checkPositionCount]
		{
			cveci2(-1,0),
			cveci2(1,0),
			cveci2(0,-1),
			cveci2(0,1)
		};
		cint maxValue = 100;
		//bool hasBlock = false;
		for (cveci2 relativeCheckPosition : relativeCheckpositions)
		{
			cveci2 absoluteCheckPosition = position + relativeCheckPosition;

			const blockID& adjacentBlock = containerIn->getBlockID(absoluteCheckPosition);

			if (adjacentBlock == blockID::tnt)
			{
				containerIn->fuseTNT(absoluteCheckPosition);
			}
			else
			{
				cint flammability = blockList[adjacentBlock]->flammability;
				if (flammability && rand(currentRandom, maxValue) < flammability)
				{
					//burn away
					containerIn->setBlockID(absoluteCheckPosition, blockID::fire, chunkLoadLevel::updateLoaded);
				}
			}
		}
		for (spreadPosition.y = position.y - 1; spreadPosition.y <= position.y + 4; spreadPosition.y++)
		{
			for (spreadPosition.x = position.x - 1; spreadPosition.x <= position.x + 1; spreadPosition.x++)
			{
				if (containerIn->getBlockID(spreadPosition) == blockID::air)
				{
					for (cveci2 relativeCheckPosition : relativeCheckpositions)
					{
						if (rand(currentRandom, maxValue) < containerIn->getBlock(spreadPosition + relativeCheckPosition)->fireEncouragement)
						{
							//spread
							containerIn->setBlockID(spreadPosition, blockID::fire, chunkLoadLevel::updateLoaded);
							break;
						}
					}
				}
			}
		}
		if (!keepsFireBurning(containerIn->getBlockID(position + cveci2(0, -1))))
		{
			age++;
		}
	}
	else
	{
		containerIn->setBlockID(position, blockID::air, chunkLoadLevel::updateLoaded);
	}
}

void fireData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	ageData::serializeValue(s);
}