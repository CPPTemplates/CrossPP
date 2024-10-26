#include "panicAI.h"
#include "mobDamageSource.h"
#include "dimension.h"
#include "mob.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeUUID.h"
bool panicAI::shouldExecute()
{
	for (std::shared_ptr<damageSource> source : connectedEntity->lastDamageSources)
	{
		if (source.get()->type == mobDamage)
		{
			uuidFleeingFrom = std::static_pointer_cast<mobDamageSource>(source).get()->uuidFrom;
		}
	}
	return uuidFleeingFrom;
}

bool panicAI::continueExecuting()
{
	if (shouldExecute())
	{
		cint chanceToForget = connectedEntity->entityType == entityID::ender_dragon ? 0x10 : 0x100;
		if (randChance(currentRandom, chanceToForget))
		{
			uuidFleeingFrom = uuid();
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

void panicAI::updateTask()
{
	mob* connectedMob = (mob*)connectedEntity;
	entity* entityFleeingFrom = connectedEntity->dimensionIn->findUUID(connectedEntity->position, fleeingSearchRadius, uuidFleeingFrom);

	if (entityFleeingFrom)
	{
		cvec2 difference = entityFleeingFrom->position - connectedEntity->position;
		//the entity is nearby
		connectedMob->wantsToSprint = true;

		vec2 destination;

		for (size_t i = 0; i < 2; i++)
		{
			destination[i] = connectedEntity->position[i] + ((difference[i] > 0) ? -0x20 : 0x20);
		}

		connectedMob->goToPosition(destination);

		if (difference.lengthSquared() > math::squared(4) && randChance(currentRandom, 0x80))
		{
			ticksSinceLookingBack = rand(currentRandom, -0x20, -0x10);
		}
	}

	connectedMob->flipBodyToWalkingDirection();
	if (entityFleeingFrom && ticksSinceLookingBack < 0)
	{
		connectedMob->lookAtEntity(entityFleeingFrom);
	}
	else
	{
		//just look forward
		connectedMob->lookForward();

	}
	ticksSinceLookingBack++;
}

void panicAI::resetTask()
{
	ticksSinceLookingBack = 0;
}

void panicAI::serializeValue(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"uuid fleeing from"), uuidFleeingFrom);
	s.serializeValue(std::wstring(L"ticks since looking back"), ticksSinceLookingBack);
}