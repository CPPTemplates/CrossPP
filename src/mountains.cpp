#include "mountains.h"
#include "dimension.h"
#include "include/math/random/random.h"
mountains::mountains() : overWorldBiomeGenerator(biomeID::mountains)
{

	std::vector<fp> octaveWeights
	{
		//big scale noise: 50%
		0.5,
		//small scale noise: 50%
		0.25,
		0.15,
		0.1,
	};
	cfp averageHillWidth = 0x80;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(0x80)));

	std::vector<fp> coverageOctaveWeights
	{
		//big scale noise: 100%
		1,
		//small scale noise: 0%
	};
	cfp averageNoiseHillWidth = 0x40;
	coverageNoise = new layerNoiseSimplex(worldRandom, coverageOctaveWeights, averageNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));

}

void mountains::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if (dimensionIn->getBlockID(pos + cveci2(0, -1)) == blockID::stone)
	{
		fp weakness = coverageNoise->evaluate(vec1(pos.x));
		if (pos.y > snowHeight)
		{
			dimensionIn->replaceBlock(pos, blockID::snow, { blockID::air });
		}
		if (weakness < 0.5)
		{
			generateTopping(dimensionIn, pos, { blockID::stone }, blockID::dirt);
			generateGrassTopping(dimensionIn, pos, randomToUse);
			if (randChance(randomToUse, 0x8))
			{
				if (pos.y <= snowHeight)
				{
					placeTree(dimensionIn, pos, woodTypeID::oak, true, randomToUse);
				}
				else
				{
					placeTree(dimensionIn, pos, woodTypeID::spruce, true, randomToUse);
				}
			}
		}
		else if (weakness < 0.6)
		{
			generateTopping(dimensionIn, pos, { blockID::stone }, blockID::gravel);
		}
	}
}