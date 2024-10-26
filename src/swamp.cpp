#include "swamp.h"
#include "dimension.h"
#include "include/math/random/random.h"
swamp::swamp() :overWorldBiomeGenerator(biomeID::swamp)
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
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, crectangle1::fromOppositeCorners(cvec1(-0x10), cvec1(0x10)));
}

void swamp::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	//block caves, so they do not appear weird without block updates
	generateTopping(dimensionIn, pos, { blockID::stone, blockID::air }, pos.y < seaLevel - 0x10 ? blockID::gravel : blockID::dirt, randomToUse);
	if (pos.y >= seaLevel)
	{
		if (shouldPlaceSugarCane(pos, randomToUse))
		{
			placeSugarCane(dimensionIn, pos, randomToUse);
		}
		generateGrassTopping(dimensionIn, pos, randomToUse);
		if (randChance(randomToUse, 0x4))
		{
			placeTreeStructure(dimensionIn, pos, std::wstring(L"swamp"), true, randomToUse);
		}
	}
	else
	{
		//fill with water
		dimensionIn->replaceBlockRange(pos, cveci2(pos.x, seaLevel - 1), blockID::water, { blockID::air });

		if (randChance(randomToUse, 0x18))
		{
			dimensionIn->addOres(cveci2(pos.x, pos.y - 1), blockID::clay, rand(randomToUse, 2, 0x10), randomToUse, { blockID::stone, blockID::dirt });
		}
	}
}