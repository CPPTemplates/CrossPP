#include "deathValley.h"
#include "dimension.h"
deathValley::deathValley() : overWorldBiomeGenerator(biomeID::death_valley)
{

	std::vector<fp> octaveWeights{
		// big scale noise: 50%
		0.5,
		// small scale noise: 50%
		0.25,
		0.15,
		0.1,
	};
	cfp averageHillWidth = 0x80;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(0x20)));
}

void deathValley::attemptgenerateStructures(dimension *dimensionIn, cveci2 &pos, std::mt19937 &randomToUse) const
{
	dimensionIn->setBlockID(pos, blockID::bonebed_dirt);
	if (dimensionIn->getBlockID(pos + cveci2(0, -1)) == blockID::stone)
	{
		generateTopping(dimensionIn, pos, {blockID::stone}, blockID::bonebed_dirt);
	}
	
}