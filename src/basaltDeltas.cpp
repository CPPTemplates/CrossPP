#include "basaltDeltas.h"
#include "dimension.h"
#include "math/random/random.h"
basaltDeltas::basaltDeltas() : netherBiomeGenerator(biomeID::basalt_deltas)
{
	const std::vector<fp>& octaveWeights
	{
		1
	};
	cfp averageHillWidth = 0x10;
	basaltNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));
}

void basaltDeltas::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	cfp basaltNoiseValue = basaltNoise->evaluate(cvec2(pos));
	if (basaltNoiseValue > 0.4)
	{
		generateTopping(dimensionIn, pos, { blockID::netherrack }, blockID::basalt);

		if (basaltNoiseValue > 0.7)
		{
			cint pillarHeight = rand(currentRandom, 0, 4);

			if (pillarHeight)
			{
				dimensionIn->replaceBlockRange(pos, pos + cveci2(0, pillarHeight - 1), blockID::basalt, { blockID::air });
			}
		}
	}
	else
	{
		generateTopping(dimensionIn, pos, { blockID::netherrack }, blockID::blackstone);
	}
}