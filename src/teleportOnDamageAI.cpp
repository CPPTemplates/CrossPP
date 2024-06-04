#include "teleportOnDamageAI.h"
bool teleportOnDamageAI::shouldExecute()
{
	if (connectedEntity->lastDamageSources.size())
	{
		auto it = connectedEntity->lastDamageSources.end();
		it--;
		const std::shared_ptr<damageSource>& source = *it;
		if (source.get()->damageTick > lastDamageTick)
		{
			lastDamageTick = source.get()->damageTick;
			return true;
		}
	}
	return false;
}

bool teleportOnDamageAI::continueExecuting()
{
	return shouldExecute();
}

void teleportOnDamageAI::updateTask()
{
	connectedEntity->teleportRandomly();
}