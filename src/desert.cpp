#include "desert.h"
#include "dimension.h"
#include "include/math/random/random.h"
desert::desert() : overWorldBiomeGenerator(biomeID::desert)
{

	std::vector<fp> octaveWeights
	{
		//big scale noise: 100%
		0b10,
		0b1
		//small scale noise: 0%
	};
	cfp averageHillWidth = 0x40;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(0x18)));
}
void desert::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	generateTopping(dimensionIn, pos, { blockID::stone }, blockID::sand, randomToUse);

	if (shouldPlaceSugarCane(pos, randomToUse))
	{
		placeSugarCane(dimensionIn, pos, randomToUse);
	}
	else
	{
		if (randChance(randomToUse, 0x10))
		{
			placeCactus(dimensionIn, pos, randomToUse);
		}
	}
}