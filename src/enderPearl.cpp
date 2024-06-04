#include "enderPearl.h"
#include "dimension.h"
enderPearl::enderPearl(dimension* dimensionIn, cvec2& position) : throwable(dimensionIn, position, entityID::ender_pearl)
{

}

void enderPearl::onCollision(const std::vector<entity*>& collidingEntities)
{
	despawn = true;
	//expand ender pearl hitbox to player hitbox
	vec2 teleportPosition;

	constexpr fp maxTeleportDistance = 0x100;
	entity* entityTeleporting = dimensionIn->findUUID(position, maxTeleportDistance, thrownFrom);

	if (entityTeleporting && dimensionIn->fitExpandingHitbox(entityTeleporting->relativeHitbox, position, teleportPosition))
	{
		entityTeleporting->teleportTo(dimensionIn, teleportPosition, true);
	}
}