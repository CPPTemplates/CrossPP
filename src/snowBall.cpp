#include "snowBall.h"
snowBall::snowBall(dimension* dimensionIn, cvec2& position) :throwable(dimensionIn, position, entityID::snowball)
{

}
void snowBall::onCollision(const std::vector<entity*>& collidingEntities)
{
	addImpactDamage(collidingEntities, 0);
	despawn = true;
}