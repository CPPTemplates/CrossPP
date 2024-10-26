#include "overWorldBiomeGenerator.h"
#include "include/math/random/random.h"
overWorldBiomeGenerator::overWorldBiomeGenerator(const biomeID& identifier) :biomeGenerator(identifier)
{
}

fp overWorldBiomeGenerator::getHeight(cint& x) const
{
	return elevationNoise->evaluate(vec1(x));
}

overWorldBiomeGenerator::~overWorldBiomeGenerator()
{

	delete elevationNoise;
}

bool overWorldBiomeGenerator::shouldPlaceSugarCane(cveci2& pos, std::mt19937& randomToUse)
{
	return pos.y < sugarCaneSpawnHeight && randChance(currentRandom, (pos.y - seaLevel) + 2);
}
