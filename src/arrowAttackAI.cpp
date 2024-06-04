#include "arrowAttackAI.h"
#include "dimension.h"
#include "humanoid.h"
void arrowAttackAI::updateTask()
{
	entity* targetEntity = connectedEntity->dimensionIn->findUUID(connectedEntity->position, proximity + mobSizeMargin, target);

	if (targetEntity && ((targetEntity->position - targetEntity->position).lengthSquared() < math::squared(proximity)))
	{
		humanoid* connectedHumanoid = (humanoid*)connectedEntity;
		mob* targetMob = (mob*)targetEntity;
		cvec2 connectedEntityHeadPosition = connectedHumanoid->getHeadPosition();
		cvec2 targetHeadPosition = targetMob->getHeadPosition();
		//can detect a player from this far away
		connectedHumanoid->lookingAt = cvec2(targetHeadPosition.x, targetHeadPosition.y + 0.2 * abs(targetHeadPosition.x - connectedEntityHeadPosition.x));
		connectedHumanoid->bowAnimationTime++;
		if (connectedHumanoid->bowAnimationTime >= ticksPerRealLifeSecond)
		{
			connectedHumanoid->launchItem(itemID::arrow);
			connectedHumanoid->bowAnimationTime = 0;
		}

		cfp distance = (targetEntity->position - connectedEntityHeadPosition).lengthSquared();
		if (distance > math::squared(0x10))
		{
			connectedHumanoid->goToPosition(targetEntity->position);
		}

		connectedHumanoid->flipBodyToWalkingDirection();
		connectedHumanoid->lookAtEntity(targetEntity);
	}
}

bool arrowAttackAI::shouldExecute()
{
	humanoid* connectedHumanoid = (humanoid*)connectedEntity;
	return attackOnCollideAI::shouldExecute() && (connectedHumanoid->itemHolding->count && (connectedHumanoid->itemHolding->stackItemID == itemID::bow));
}