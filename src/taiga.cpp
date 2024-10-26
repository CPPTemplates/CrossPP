#include "overWorld.h"
#include "idAnalysis.h"
#include "taiga.h"
#include "dimension.h"
#include "include/math/random/random.h"
taiga::taiga() : overWorldBiomeGenerator(biomeID::taiga)
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
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, hillyNoiseRange);

	std::vector<fp> carrotOctaveWeights
	{
		//big scale noise: 100%
		1,
		//small scale noise: 0%
	};
	cfp averageCarrotNoiseHillWidth = 0x40;
	carrotNoise = new layerNoiseSimplex(worldRandom, carrotOctaveWeights, averageCarrotNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));

	std::vector<fp> pumpkinOctaveWeights
	{
		//big scale noise: 100%
		1,
		//small scale noise: 0%
	};
	cfp averagePumpkinNoiseHillWidth = 0x40;
	pumpkinNoise = new layerNoiseSimplex(worldRandom, pumpkinOctaveWeights, averagePumpkinNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));
}

void taiga::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	constexpr fp carrotsTreshold = 0.6;
	constexpr fp pumpkinTreshold = 0.6;
	//cbool snowy = dimensionIn->identifier == dimensionID::overworld && ((overWorld*)dimensionIn)->getTemperature(pos) < snowTemperatureTreshold;
	if (dimensionIn->getBlockID(pos + cveci2(0, -1)) == blockID::stone)
	{
		generateTopping(dimensionIn, pos, { blockID::stone }, blockID::dirt);
		if (randChance(randomToUse, 0x4))
		{
			dimensionIn->setBlockID(pos + cveci2(0, -1), blockID::podzol);
			placeTree(dimensionIn, pos, woodTypeID::spruce, true, randomToUse);
		}
		else if ((pumpkinNoise->evaluate(vec1(pos.x)) > pumpkinTreshold) && randChance(currentRandom, 0x4))
		{
			dimensionIn->setBlockID(pos + cveci2(0, -1), blockID::podzol);
			placeRandomlyGrownStemPlant(dimensionIn, pos, blockID::pumpkin, randomToUse);
		}
		else if (carrotNoise->evaluate(vec1(pos.x)) > carrotsTreshold)
		{
			cbool& onBiomeBorder = dimensionIn->getBiome(pos + cveci2(0, -1)) != identifier || dimensionIn->getBiome(pos + cveci2(0, 1)) != identifier;
			cbool& onFarmlandBorder = carrotNoise->evaluate(vec1(pos.x - 1)) <= carrotsTreshold || carrotNoise->evaluate(vec1(pos.x + 1)) <= carrotsTreshold;

			if ((onBiomeBorder || onFarmlandBorder) && randChance(randomToUse, 2))
			{
				dimensionIn->replaceBlock(pos, blockID::podzol, { blockID::dirt });
				//bush or tree
				if (randChance(randomToUse, 2)) //tree
				{
					placeTree(dimensionIn, pos, woodTypeID::spruce, true, randomToUse);
				}
				else //bush
				{
					dimensionIn->replaceBlock(pos, getTreeBlock(woodTypeID::spruce, treeItemTypeID::leaves), { blockID::air });
				}
			}
			else
			{
				//generate farmland
				placeRandomlyGrownCrop(dimensionIn, pos, randChance(randomToUse, 0x4) ? blockID::potatoes : blockID::carrots, randomToUse);
			}
		}
		else
		{
			if (dimensionIn->identifier == dimensionID::overworld && ((overWorld*)dimensionIn)->getTemperature(pos) < snowTemperatureTreshold)
			{
				dimensionIn->replaceBlock(pos, blockID::snow, { blockID::air });
				dimensionIn->replaceBlock(pos + cveci2(0, -1), blockID::podzol, { blockID::dirt });
			}
			else
			{
				generateGrassTopping(dimensionIn, pos, randomToUse, blockID::fern);
			}
		}
	}
}

taiga::~taiga()
{

	delete carrotNoise;
}