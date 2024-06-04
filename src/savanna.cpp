#include "savanna.h"
#include "dimension.h"
#include "include/math/random/random.h"
savanna::savanna() : overWorldBiomeGenerator(biomeID::savanna)
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
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, flatNoiseRange);
}

void savanna::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if (dimensionIn->getBlockID(pos + cveci2(0, -1)) == blockID::stone)
	{
		generateTopping(dimensionIn, pos, { blockID::stone }, blockID::dirt);
		if (randChance(randomToUse, 0x20))
		{
			dimensionIn->setBlockID(cveci2(pos) + cveci2(0, -1), blockID::grass_block);
			placeTree(dimensionIn, pos, woodTypeID::acacia, true, randomToUse);
		}
		else
		{
			generateGrassTopping(dimensionIn, pos, randomToUse);
		}
	}
}