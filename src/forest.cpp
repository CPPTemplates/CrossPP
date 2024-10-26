#include "forest.h"
#include "dimension.h"
#include "math/random/random.h"
forest::forest() : overWorldBiomeGenerator(biomeID::forest)
{

	const std::vector<fp>& octaveWeights
	{
		//big scale noise: 40%
		0.4,
		//small scale noise: 60%
		0.3,
		0.2,
		0.1,
	};
	cfp averageHillWidth = 0x40;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, hillyNoiseRange);
}

void forest::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if (dimensionIn->getBlockID(pos + cveci2(0, -1)) == blockID::stone)
	{
		generateTopping(dimensionIn, pos, { blockID::stone }, blockID::dirt);
		if (randChance(randomToUse, 0x4))
		{
			dimensionIn->setBlockID(pos + cveci2(0, -1), blockID::grass_block);
			placeTree(dimensionIn, pos, randChance(randomToUse, 2) ? woodTypeID::oak : woodTypeID::birch, true, randomToUse);
		}
		else
		{
			if (randChance(randomToUse, 0x20))
			{
				//generate farmland with carrots
				placeRandomlyGrownCrop(dimensionIn, pos, blockID::carrots, randomToUse);
			}
			else
			{
				generateGrassTopping(dimensionIn, pos, randomToUse);
			}
		}
	}
}