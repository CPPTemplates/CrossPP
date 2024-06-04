#include "idAnalysis.h"
#include "grassBlockData.h"
#include "tickableBlockContainer.h"
#include "dimension.h"
#include "math/random/random.h"
void grassBlockData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	const blockID& topBlock = containerIn->getBlockID(position + cveci2(0, 1));

	if (diesWithoutLight(containerIn->getBlockID(position)) && (containerIn->rootDimension->getMaximumInternalLightLevel(position + cveci2(0, 1)) < glowInTheDarkLightLevel))
	{
		//die
		containerIn->setBlockID(position, blockID::dirt);
	}
	else
	{
		if ((topBlock == blockID::air) && randChance(currentRandom, 0x8))
		{
			containerIn->setBlockID(position + cveci2(0, 1), getGrassToGrowOn(containerIn->getBlockID(position)));
		}
	}
}