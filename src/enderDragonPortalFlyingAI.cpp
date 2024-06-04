#include "enderDragonPortalFlyingAI.h"
#include "enderDragon.h"
#include "dimension.h"
#include "end.h"
#include "soundList.h"
bool enderDragonPortalFlyingAI::shouldExecute()
{
	return connectedEntity->health <= 0;
}

void enderDragonPortalFlyingAI::updateTask()
{
	enderDragon* connectedEnderdragon = (enderDragon*)connectedEntity;

	bool shouldPlayAnimation = (connectedEntity->dimensionIn->identifier != dimensionID::end) || connectedEnderdragon->deathAnimationTicks;

	if (!shouldPlayAnimation)
	{
		cvec2 dyingLocation = cvec2(0, ((end*)connectedEntity->dimensionIn)->exitPortalLevel) + enderDragonRelativeDyingLocation;
		cfp distanceToDyingLocationSquared = (connectedEnderdragon->position - dyingLocation).lengthSquared();

		if ((distanceToDyingLocationSquared < math::squared(0x2)) || ((distanceToDyingLocationSquared > math::squared(maximalEnderDragonDyingAnimationDistance))))
		{
			shouldPlayAnimation = true;
		}
		else
		{
			if (connectedEnderdragon->deathAnimationTicks == 0)
			{
				connectedEnderdragon->goToPosition(dyingLocation);
			}
		}
	}

	if (shouldPlayAnimation)
	{
		if (connectedEnderdragon->deathAnimationTicks == 0)
		{
			enderDragonDeathSound->playRandomSound(connectedEntity->dimensionIn, connectedEntity->position);
		}
		connectedEnderdragon->speed *= 0.8;
		connectedEnderdragon->speed.y += enderDragonDyingAscendSpeed * (1 - 0.8);
		connectedEnderdragon->deathAnimationTicks++;
	}
}