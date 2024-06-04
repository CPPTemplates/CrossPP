#include "mushroomFields.h"
#include "dimension.h"
#include "include/math/random/random.h"
mushroomFields::mushroomFields() : overWorldBiomeGenerator(biomeID::desert)
{

	std::vector<fp> octaveWeights
	{
		//big scale noise: 100%
		0b10,
		0b1
		//small scale noise: 0%
	};
	cfp averageHillWidth = 0x40;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, flatNoiseRange);
}
void mushroomFields::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	generateGrassTopping(dimensionIn, pos, randomToUse, blockID::mycelium);
	if (randChance(randomToUse, 0x10))
	{
		placeHugeMushroom(dimensionIn, pos, randomToUse);
	}
}