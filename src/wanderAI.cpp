#include "wanderAI.h"
#include "mob.h"
#include "nbt/nbtSerializer.h"
bool wanderAI::continueExecuting()
{

	return true;
}

void wanderAI::startExecuting()
{
	targetX = connectedEntity->position.x;
}

void wanderAI::updateTask()
{
	mob* connectedMob = (mob*)connectedEntity;
	bool canReachTargetPoint = connectedMob->goToPosition(cvec2(targetX, connectedEntity->position.y));
	if (randChance(currentRandom, (!canReachTargetPoint || (abs(targetX - connectedEntity->position.x) < 1)) ? 0x10 : 0x100))
	{
		cfp targetOffset = 0x10;
		//choose new target
		targetX = connectedEntity->position.x + randFp(currentRandom, -targetOffset, targetOffset);
	}

	connectedMob->flipBodyToWalkingDirection();
	connectedMob->lookForward();
}

void wanderAI::resetTask()
{
	targetX = connectedEntity->position.x;
}

wanderAI::wanderAI(entity* connectedEntity) : taskAI(connectedEntity)
{
	resetTask();
}

void wanderAI::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"target x"), targetX);
}