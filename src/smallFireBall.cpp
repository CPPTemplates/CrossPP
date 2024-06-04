#include "smallFireBall.h"
smallFireBall::smallFireBall(dimension* dimensionIn, cvec2& position) :fireBall(dimensionIn, position, entityID::small_Fireball)
{

}

void smallFireBall::onCollision(const std::vector<entity*>& collidingEntities)
{
	fireBall::onCollision(collidingEntities);

	addImpactDamage(collidingEntities, 5);

	for (entity* e : collidingEntities)
	{
		e->setOnFire(100);
	}
}