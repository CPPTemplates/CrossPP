#include "plains.h"
#include "dimension.h"
#include "include/math/random/random.h"
plains::plains() : overWorldBiomeGenerator(biomeID::plains)
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

	std::vector<fp> farmLandOctaveWeights
	{
		//big scale noise: 100%
		1,
		//small scale noise: 0%
	};
	cfp averageNoiseHillWidth = 0x40;
	farmlandNoise = new layerNoiseSimplex(worldRandom, farmLandOctaveWeights, averageNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));
}

void plains::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if (dimensionIn->getBlockID(pos + cveci2(0, -1)) == blockID::stone)
	{
		generateTopping(dimensionIn, pos, { blockID::stone }, blockID::dirt);

		constexpr fp farmLandTreshold = 0.8;
		if (farmlandNoise->evaluate(vec1(pos.x)) > farmLandTreshold)
		{
			cbool& onBiomeBorder = dimensionIn->getBiome(pos + cveci2(0, -1)) != identifier || dimensionIn->getBiome(pos + cveci2(0, 1)) != identifier;
			cbool& onFarmlandBorder = farmlandNoise->evaluate(vec1(pos.x - 1)) <= farmLandTreshold || farmlandNoise->evaluate(vec1(pos.x + 1)) <= farmLandTreshold;
			if ((onBiomeBorder || onFarmlandBorder) && randChance(randomToUse, 2))
			{
				dimensionIn->setBlockID(pos + cveci2(0, -1), blockID::grass_block);
				//bush or tree
				if (randChance(randomToUse, 2)) //tree
				{
					placeTreeStructure(dimensionIn, pos, randChance(randomToUse, 2) ? woodTypeID::oak : woodTypeID::birch, true, randomToUse);
				}
				else //bush
				{
					dimensionIn->replaceBlock(pos, getTreeBlock(woodTypeID::oak, treeItemTypeID::leaves), { blockID::air });
				}
			}
			else
			{
				//generate farmland
				placeRandomlyGrownCrop(dimensionIn, pos, randChance(randomToUse, 0x4) ? blockID::beetroots : blockID::wheat, randomToUse);
			}
		}
		else
		{
			generateGrassTopping(dimensionIn, pos, randomToUse);
			if (randChance(randomToUse, 0x20))
			{
				placeTree(dimensionIn, pos, randChance(randomToUse, 2) ? woodTypeID::oak : woodTypeID::birch, true, randomToUse);
			}
		}
	}
}

plains::~plains()
{

	delete farmlandNoise;
}