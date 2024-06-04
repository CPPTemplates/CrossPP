#include "block.h"
#include "throwableData.h"
#include "idAnalysis.h"
#include "throwable.h"
#include "dimension.h"
#include "nbt/serializeUUID.h"
throwable::throwable(dimension* dimensionIn, cvec2& position, const entityID& entityType) :entity(dimensionIn, position, entityType)
{
}

void throwable::tick()
{
	crectangle2 currentHitbox = calculateHitBox();

	cfp& maxCollisionSpeed = 40;//the maximum speed to keep track of when getting near entities

	std::vector<entity*> entities = dimensionIn->findNearEntities(currentHitbox.expanded((maxCollisionSpeed * secondsPerTick) + mobSizeMargin));
	std::vector<entity*> collidables = std::vector<entity*>();

	for (entity* e : entities)
	{
		if (collidesWithThrowables(e->entityType) && collidesThisTick(*this, *e))
		{
			collidables.push_back(e);
		}
	}

	if (axisCollided.x || axisCollided.y)
	{
		onCollision(collidables);
	}
	else
	{
		bool collidingWithOwner = false;

		for (entity* e : collidables)
		{
			if ((!leftOwner) && (e->identifier == thrownFrom))
			{
				collidingWithOwner = true;
			}
			else if (e != this)
			{
				onCollision(collidables);
				break;
			}
		}
		if (!collidingWithOwner)
		{
			leftOwner = true;
		}
	}
	entity::tick();
}

void throwable::render(const gameRenderData& targetData) const
{
	crectangle2 hitboxRect = calculateHitBox();
	renderTexture(hitboxRect, targetData.worldToRenderTargetTransform, true, standardUpFacingBlockDirection, *((throwableData*)entityDataList[(int)entityType])->tex, targetData.renderTarget, true);
}

void throwable::onCollision(const std::vector<entity*>& collidingEntities)
{
}

void throwable::serializeValue(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"thrown from"), thrownFrom);
	s.serializeValue(std::wstring(L"left owner"), leftOwner);
}

void throwable::addImpactDamage(const std::vector<entity*>& collidingEntities, cfp& damage)
{
	for (entity* const& collidingEntity : collidingEntities)
	{
		if (collidingEntity != this)
		{
			collidingEntity->addDamageSource(damage, std::make_shared<damageSource>(projectileDamage));
			speed = collidingEntity->handleCollision(speed, getWeight());
		}
	}
}