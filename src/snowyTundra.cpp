#include "snowyTundra.h"
#include "dimension.h"
snowyTundra::snowyTundra() : overWorldBiomeGenerator(biomeID::snowy_tundra)
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
	cfp averageHillWidth = 0x40;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, flatNoiseRange);
}

void snowyTundra::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if (dimensionIn->getBlockID(pos + cveci2(0, -1)) == blockID::stone)
	{
		generateTopping(dimensionIn, pos, { blockID::stone }, blockID::dirt);

		dimensionIn->setBlockID(pos, blockID::grass_block);
		dimensionIn->setBlockID(pos + cveci2(0, 1), blockID::snow);
	}
}