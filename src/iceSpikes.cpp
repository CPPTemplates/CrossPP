#include "iceSpikes.h"
#include "dimension.h"
#include "include/math/random/random.h"
iceSpikes::iceSpikes() : overWorldBiomeGenerator(biomeID::ice_spikes)
{

	std::vector<fp> octaveWeights
	{
		//big scale noise: 100%
		1,
		//small scale noise: 0%
	};
	cfp averageHillWidth = 0x40;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(0x18)));
}

void iceSpikes::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if (dimensionIn->getBlockID(pos + cveci2(0, -1)) == blockID::stone)
	{
		dimensionIn->replaceBlockRange(cveci2(pos.x, pos.y - 5), pos + cveci2(0, -3), blockID::dirt, { blockID::stone });
		dimensionIn->replaceBlockRange(pos + cveci2(0, -2), pos + cveci2(0, -1), blockID::snow_block, { blockID::stone });
		if (randChance(randomToUse, 0x10))
		{
			placeIceSpike(dimensionIn, pos, randomToUse);
		}
	}
}