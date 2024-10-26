#include "fireBall.h"
fireBall::fireBall(dimension *dimensionIn, cvec2 &position, const entityID &entityType) : throwable(dimensionIn, position, entityType)
{
}

void fireBall::onCollision(const std::vector<entity *> &collidingEntities)
{
	addImpactDamage(collidingEntities, fireBallImpactDamage);

	despawn = true;
}

fp fireBall::getGravityForce() const
{
	return 0;
}

std::vector<vec3> fireBall::getFrictions() const
{
	return std::vector<vec3>();
}

void fireBall::tick()
{
	throwable::tick();
	ticksToLive--;
	if (ticksToLive == 0)
	{
		despawn = true;
	}
}
