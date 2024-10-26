#include "netherForest.h"
#include "dimension.h"
#include "include/math/random/random.h"
void netherForest::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	generateTopping(dimensionIn, pos, { blockID::netherrack }, (blockID)((int)blockID::crimson_nylium + ((int)identifier - (int)biomeID::crimson_forest)), 1);
	if (randChance(randomToUse, 0x10))
	{
		placeTreeStructure(dimensionIn, pos, (woodTypeID)((int)woodTypeID::crimson + ((int)identifier - (int)biomeID::crimson_forest)), true, randomToUse);
	}
	else if (randChance(randomToUse, 0x20))
	{
		dimensionIn->replaceBlock(pos, (blockID)((int)blockID::wood_sapling + (int)woodTypeID::crimson + ((int)identifier - (int)biomeID::crimson_forest)), { blockID::air });
	}
}