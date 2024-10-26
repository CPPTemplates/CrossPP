#include "soulSandValley.h"
#include "dimension.h"
#include "math/random/random.h"
soulSandValley::soulSandValley() :netherBiomeGenerator(biomeID::soulsand_valley)
{
}

void soulSandValley::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	generateTopping(dimensionIn, pos, { blockID::netherrack }, blockID::soul_sand, randomToUse);
}

void soulSandValley::attemptgenerateCeilingStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	generateCeilingTopping(dimensionIn, pos, { blockID::netherrack }, blockID::soul_soil, rand(randomToUse, 5, 7));
}