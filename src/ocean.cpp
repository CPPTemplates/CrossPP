#include "ocean.h"
#include "dimension.h"
#include "include/math/random/random.h"
ocean::ocean() : overWorldBiomeGenerator(biomeID::ocean)
{

	std::vector<fp> octaveWeights
	{
		0b1000,
		0b100,
		0b10,
		0b1,
	};
	cfp averageHillWidth = 0x80;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, crectangle1::fromOppositeCorners(cvec1(-0x100), cvec1(2)));
	std::vector<fp> kelpOctaveWeights
	{
		//big scale noise: 100%
		1,
		//small scale noise: 0%
	};
	cfp averageNoiseHillWidth = 0x20;
	kelpNoise = new layerNoiseSimplex(worldRandom, kelpOctaveWeights, averageNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));

}

void ocean::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	//block caves, so they do not appear weird without block updates
	generateTopping(dimensionIn, pos, { blockID::stone, blockID::air }, pos.y < seaLevel - 0x10 ? blockID::gravel : blockID::sand, randomToUse);
	if (pos.y >= seaLevel)
	{
		if (randChance(randomToUse, 0x10))
		{
			placeTreeStructure(dimensionIn, pos, std::wstring(L"palm"), true, randomToUse);
		}
	}
	else
	{
		//fill with water
		dimensionIn->replaceBlockRange(pos, cveci2(pos.x, seaLevel - 1), blockID::water, { blockID::air });
		if (kelpNoise->evaluate(vec1(pos.x)) > 0.6)
		{
			//kelp plants
			if (pos.y < seaLevel - 0x10 && randChance(randomToUse, 0x4))
			{
				cint plantHeight = rand(randomToUse, 0x8, 0x20);
				dimensionIn->replaceBlockRange(pos, cveci2(pos.x, math::minimum(pos.y + plantHeight - 1, seaLevel - 1)), blockID::kelp, { blockID::air });
			}
		}
	}
}