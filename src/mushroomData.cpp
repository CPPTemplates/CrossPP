#include "mushroomData.h"
#include "tickableBlockContainer.h"
#include "biomeGenerator.h"
#include "include/math/random/random.h"
void mushroomData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (randChance(currentRandom, 0x10))
	{
		//the sapling will be replaced by the trunk of the tree
		const blockID linkedBlockID = containerIn->getBlockID(position);
		mushroomColorID mushroomType = (mushroomColorID)((int)linkedBlockID - (int)blockID::red_mushroom);
		biomeGenerator::placeHugeMushroom(containerIn, position, mushroomType, currentRandom);
	}
}