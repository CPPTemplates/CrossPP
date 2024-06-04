#include "endLands.h"
#include "dimension.h"
#include "include/math/random/random.h"
endLands::endLands() : endBiomeGenerator(biomeID::end_lands)
{
}

void endLands::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	if (randChance(worldRandom, 0x10))
	{
		placeChorusTree(dimensionIn, pos, randomToUse);
	}
}