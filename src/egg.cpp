#include "egg.h"
egg::egg(dimension* dimensionIn, cvec2& position) : throwable(dimensionIn, position, entityID::egg)
{
}
void egg::onCollision(const std::vector<entity*>& collidingEntities)
{
	addImpactDamage(collidingEntities, 0);
	despawn = true;
}