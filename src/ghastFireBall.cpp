#include "ghastFireBall.h"
#include "dimension.h"
ghastFireBall::ghastFireBall(dimension* dimensionIn, cvec2& position) :fireBall(dimensionIn, position, entityID::ghast_fireball)
{
}

void ghastFireBall::onCollision(const std::vector<entity*>& collidingEntities)
{
	fireBall::onCollision(collidingEntities);
	dimensionIn->createExplosion(position, 1, true);

}