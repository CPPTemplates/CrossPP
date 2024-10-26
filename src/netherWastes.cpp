#include <numeric>
#include "netherWastes.h"
#include "dimension.h"
#include "include/math/random/random.h"
netherWastes::netherWastes() :netherBiomeGenerator(biomeID::nether_wastes)
{
	std::vector<fp> fireOctaveWeights
	{
		//big scale noise: 100%
		1,
		//small scale noise: 0%
	};
	cfp averageNoiseHillWidth = 0x40;
	fireNoise = new layerNoiseSimplex(worldRandom, fireOctaveWeights, averageNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));

}

void netherWastes::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if ((fireNoise->evaluate(cvec2(pos)) > 0.8) && randChance(randomToUse, 0x2))
	{
		dimensionIn->replaceBlock(pos, blockID::fire, { blockID::air });
	}

	if (randChance(randomToUse, 0x20))
	{

		std::vector<fp> mushroomWeights = { 0.4, 0.6 };

		dimensionIn->replaceBlock(pos, (blockID)((int)blockID::red_mushroom + randIndex(randomToUse, mushroomWeights, std::accumulate(mushroomWeights.begin(), mushroomWeights.end(), (fp)0))), { blockID::air });
	}
}