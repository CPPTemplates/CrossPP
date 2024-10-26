#include "shortGrassData.h"
#include "tickableBlockContainer.h"
#include "include/math/random/random.h"
void shortGrassData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	pollenData::randomTick(containerIn, position);
	if (randChance(currentRandom, 0x8))
	{
		if (containerIn->getBlockID(position + cveci2(0, 1)) == blockID::air)
		{
			containerIn->setBlockID(position, (blockID)((int)blockID::tall_grass + ((int)containerIn->getBlockID(position) - (int)blockID::grass)));
		}
	}
}