#include "jungle.h"
#include "dimension.h"
#include "include/math/random/random.h"

jungle::jungle() : overWorldBiomeGenerator(biomeID::jungle)
{

	std::vector<fp> octaveWeights
	{
		//big scale noise: 40%
		0.4,
		//small scale noise: 60%
		0.3,
		0.2,
		0.1,
	};
	cfp averageHillWidth = 0x20;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, hillyNoiseRange);

	std::vector<fp> carrotOctaveWeights
	{
		//big scale noise: 100%
		1,
		//small scale noise: 0%
	};
	cfp averageNoiseHillWidth = 0x20;
	melonNoise = new layerNoiseSimplex(worldRandom, carrotOctaveWeights, averageNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));
}

void jungle::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if (dimensionIn->getBlockID(cveci2(pos.x, pos.y - 1)) == blockID::stone)
	{
		generateTopping(dimensionIn, pos, { blockID::stone }, blockID::dirt);

		if (randChance(randomToUse, 0x4))
		{
			placeTree(dimensionIn, pos, woodTypeID::jungle, true, randomToUse);
		}
		else if (randChance(randomToUse, 0x4) && (melonNoise->evaluate(cvec2(pos)) > 0.8))
		{
			placeRandomlyGrownStemPlant(dimensionIn, pos, blockID::melon, randomToUse);
		}
		else
		{
			generateGrassTopping(dimensionIn, pos, randomToUse);
		}
	}
}