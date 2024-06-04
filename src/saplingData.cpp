#include "saplingData.h"
#include "tickableBlockContainer.h"
#include "biomeGenerator.h"
#include "include/math/random/random.h"
saplingData::saplingData()
{
}

void saplingData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (randChance(currentRandom, 0x10))
	{
		//the sapling will be replaced by the trunk of the tree
		const blockID linkedBlockID = containerIn->getBlockID(position);
		woodTypeID saplingType = (woodTypeID)((int)linkedBlockID - (int)blockID::wood_sapling);
		biomeGenerator::placeTree(containerIn, position, saplingType, false, currentRandom);
	}
}