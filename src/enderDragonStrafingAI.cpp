#include "enderDragonStrafingAI.h"
#include "dimension.h"
#include "enderDragon.h"

bool enderDragonStrafingAI::shouldExecute()
{
	return fireBallAI::shouldExecute() && //(currentPlayableCharachter->dimensionIn->identifier == dimensionID::end) &&
		((enderDragon*)connectedEntity)->endCrystalBroken;
}

bool enderDragonStrafingAI::continueExecuting()
{
	return shouldExecute();
}

void enderDragonStrafingAI::updateTask()
{
	enderDragon* connectedEnderDragon = ((enderDragon*)connectedEntity);
	connectedEnderDragon->flying = true;
	if (entity* target = findTarget()) {
		cvec2 targetPoint = target->position;//TODO: make the dragon fly to the target, THEN shoot a fireball
		//cfp distance = (connectedEnderDragon->position - targetPoint).lengthSquared();
		//if (targetInLineOfSight(enderDragonSightRange))
		//{
			//shoot fireball
		shootFireBall(target);
		connectedEnderDragon->endCrystalBroken = false;
		//}

		connectedEnderDragon->goToPosition(targetPoint);
		connectedEnderDragon->flipBodyToWalkingDirection();
	}
}
