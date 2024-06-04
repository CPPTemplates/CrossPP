#include "explodeAI.h"
#include "soundList.h"
#include "dimension.h"
#include "creeper.h"
void explodeAI::startExecuting()
{
	fuseSound->playRandomSound(connectedEntity->dimensionIn, connectedEntity->position, 4);
}

bool explodeAI::continueExecuting()
{
	return shouldExecute();
}

void explodeAI::updateTask()
{
	attackOnCollideAI::updateTask();
	creeper* connectedCreeper = dynamic_cast<creeper*>(connectedEntity);
	connectedCreeper->ticksUntilFuse--;
	if (connectedCreeper->ticksUntilFuse == 0)
	{
		connectedCreeper->despawn = true;
		connectedEntity->dimensionIn->createExplosion(connectedEntity->position, connectedCreeper->explosionStrength * (connectedCreeper->powered ? 2 : 1));
	}
}

void explodeAI::resetTask()
{
	attackOnCollideAI::resetTask();
	dynamic_cast<creeper*>(connectedEntity)->ticksUntilFuse = creeperFuseLength;
}