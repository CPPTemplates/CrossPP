#include "cropData.h"
#include "tickableBlockContainer.h"
#include "cropBlock.h"
#include "math/random/random.h"
void cropData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	cropBlock* linkedBlock = ((cropBlock*)containerIn->getBlock(position));
	cint lastGrowthStage = linkedBlock->growthStageCount - 1;
	if (age < lastGrowthStage && randFp(currentRandom) < linkedBlock->chanceToGrow)
	{
		age++;
	}
}

void cropData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	ageData::serializeValue(s);
}