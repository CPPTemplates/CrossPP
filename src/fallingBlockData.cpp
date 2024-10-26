#include "fallingBlockData.h"
#include "tickableBlockContainer.h"
#include "block.h"
#include "fallingBlock.h"
bool fallingBlockData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	block* blockBelow = containerIn->getBlock(position + cveci2(0, -1));
	if (blockBelow->blockCollisionType == collisionTypeID::willNotCollide)
	{
		fallingBlock* entityForm = (fallingBlock*)summonEntity(entityID::falling_block, containerIn, cvec2(position.x + 0.5, position.y));
		entityForm->linkedBlockID = containerIn->getBlockID(position);
		entityForm->onGround = false;
		containerIn->setBlockID(position, blockID::air, chunkLoadLevel::updateLoaded);
	}
	return false;
}