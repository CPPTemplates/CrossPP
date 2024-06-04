#include "attackOnCollideAI.h"
#include "dimension.h"
#include "idConverter.h"
#include "math/random/random.h"
entity* attackOnCollideAI::findNearestAttackableTarget() const
{
	std::vector<entity*> nearEntities = connectedEntity->dimensionIn->findNearEntities(connectedEntity->position, proximity);

	entity* nearestEntity = nullptr;
	fp nearestDistanceSquared = INFINITY;

	for (entity* e : nearEntities)
	{
		if (e->entityType == entityTypeToAttack)
		{
			cfp& currentDistanceSquared = (e->position - connectedEntity->position).lengthSquared();
			if (currentDistanceSquared < nearestDistanceSquared)
			{
				if (targetNear(e))
				{
					nearestDistanceSquared = currentDistanceSquared;
					nearestEntity = e;
				}
			}
		}
	}
	return nearestEntity;
}

bool attackOnCollideAI::shouldExecute()
{
	if (targetNear())
	{
		return true;
	}
	else
	{
		entity* targetEntity = findNearestAttackableTarget();
		if (targetEntity)
		{
			target = targetEntity->identifier;
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool attackOnCollideAI::continueExecuting()
{
	cint& reactionTimeSeconds = 2;
	return randChance(worldRandom, reactionTimeSeconds * ticksPerRealLifeSecond) ? shouldExecute() : true;
}

void attackOnCollideAI::serializeValue(nbtSerializer& s)
{
	targetAI::serializeValue(s);
	idConverter::serializeID(s, std::wstring(L"entity type to attack"), entityTypeToAttack, s.converter ? &s.converter->entityIDConverter : nullptr);
}