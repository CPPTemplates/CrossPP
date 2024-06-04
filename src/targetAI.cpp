#include "targetAI.h"
#include "mob.h"
#include "dimension.h"
#include "human.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeUUID.h"
bool targetAI::shouldExecute()
{
	return targetNear();
}

bool targetAI::targetNear(const entity* targetEntity, fp proximity) const
{
	if (!proximity) proximity = this->proximity;
	cfp distanceToTargetSquared = (targetEntity->position - connectedEntity->position).lengthSquared();
	if (distanceToTargetSquared < math::squared(proximity))
	{
		if (targetEntity->entityType != entityID::human || willBeNoticedByMobs(((human*)targetEntity)->currentGameMode))
		{

			mob* connectedMob = (mob*)connectedEntity;
			mob* targetMob = (mob*)targetEntity;
			cvec2 eyePosition = connectedMob->getHeadPosition();
			cvec2& targetHeadPosition = targetMob->getHeadPosition();
			cvec2 relative = targetHeadPosition - eyePosition;
			if (relative.lengthSquared() > math::squared(proximity))
			{
				return false;
			}

			veci2 blockPos, adjacentBlockPosition;
			vec2 exactBlockIntersection;
			tickableBlockContainer* resultingContainer;

			connectedMob->dimensionIn->findRaycastRecursive(eyePosition, targetHeadPosition, blockPos, adjacentBlockPosition, exactBlockIntersection, resultingContainer);
			return exactBlockIntersection == targetHeadPosition;
		}
	}
	return false;
}

bool targetAI::targetNear() const
{
	if (target)
	{
		entity* targetEntity = connectedEntity->dimensionIn->findUUID(connectedEntity->position, proximity + mobSizeMargin, target);
		if (targetEntity)
		{
			return targetNear(targetEntity);
		}
	}
	return false;
}

entity* targetAI::findTarget() const
{
	return connectedEntity->dimensionIn->findUUID(connectedEntity->position, proximity + mobSizeMargin, target);
}

void targetAI::updateTask()
{
	entity* targetEntity = connectedEntity->dimensionIn->findUUID(connectedEntity->position, proximity + mobSizeMargin, target);

	if (targetEntity && ((targetEntity->position - targetEntity->position).lengthSquared() < math::squared(proximity)))
	{
		mob* connectedMob = (mob*)connectedEntity;
		//can detect a player from this far away

		if (((mob*)connectedEntity)->ticksSinceToolUsed >= ((mob*)connectedEntity)->getTotalCoolDownTime())
		{
			//performance heavy
			connectedMob->updateSelection();
			if (connectedMob->selectedUUID == targetEntity->identifier)
			{
				connectedMob->wantsToHit = true;
			}
			else if (connectedMob->selectedUUID)
			{
				//tries to jump to hit the player over other entities
				connectedMob->wantsToJump = true;
			}
		}
		cbool couldHitTarget = connectedMob->selectedUUID == targetEntity->identifier;
		cfp distance = (targetEntity->position - connectedMob->position).lengthSquared();
		if (distance > math::squared(2) || !couldHitTarget)
		{
			connectedMob->goToPosition(targetEntity->position);
		}

		connectedMob->flipBodyToWalkingDirection();
		connectedMob->lookAtEntity(targetEntity);
	}
}

void targetAI::serializeValue(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"target"), target);
	//proximity will be set by the initializer and doesn't have to be serialized
}