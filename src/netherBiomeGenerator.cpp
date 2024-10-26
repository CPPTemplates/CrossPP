#include "netherBiomeGenerator.h"
#include "dimension.h"
#include "include/math/random/random.h"
netherBiomeGenerator::netherBiomeGenerator(const biomeID& identifier) :biomeGenerator(identifier)
{
}

void netherBiomeGenerator::attemptgenerateCeilingStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if (randChance(randomToUse, 0x20))
	{
		dimensionIn->addOres(cveci2(pos.x, pos.y - 1), blockID::glowstone, rand(randomToUse, 0x4, 0x10), randomToUse, { blockID::air });
	}
}