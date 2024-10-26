#include "ridableEntity.h"
#include "dimension.h"
#include "mob.h"
#include "idAnalysis.h"
#include "math/collisions.h"
#include "nbt/serializeUUID.h"
bool ridableEntity::addPassenger(mob* m)
{
	for (uuid& seat : seats)
	{
		if (!seat)
		{
			seat = m->identifier;
			m->UUIDRidingOn = identifier;
			return true;
		}
	}
	return false;
}

void ridableEntity::serializeValue(nbtSerializer& s)
{
	entity::serializeValue(s);
	if (s.push<nbtDataTag::tagList>(std::wstring(L"riding uuids")))
	{
		if (s.write)
		{
			for (uuid& seat : seats)
			{
				if (s.push<nbtDataTag::tagCompound>())
				{
					serializeNBTValue(s, std::wstring(L"uuid"), seat);
					s.pop();
				}
			}
		}
		else
		{
			std::vector<nbtData*> data = s.getChildren();
			for (size_t i = 0; (i < seats.size()) && (i < data.size()); i++)
			{
				if (s.push(data[i]))
				{
					serializeNBTValue(s, std::wstring(L"uuid"), seats[i]);
					s.pop();
				}
			}
		}
		s.pop();
	}
}

ridableEntity::ridableEntity(dimension* dimensionIn, cvec2& position, const entityID& entityType, cint& seatCount) : entity(dimensionIn, position, entityType), seats(std::vector<uuid>(seatCount))
{
}

void ridableEntity::addCollidingEntities()
{
	crectangle2 currentHitbox = calculateHitBox();
	const std::vector<entity*> possiblyCollidingEntities = dimensionIn->findNearEntities(currentHitbox.expanded(mobSizeMargin));

	for (entity* const& possiblyCollidingEntity : possiblyCollidingEntities)
	{
		if (isMob(possiblyCollidingEntity->entityType))
		{
			crectangle2 hitbox = possiblyCollidingEntity->calculateHitBox();
			//entity must fit inside
			if (hitbox.size.x < currentHitbox.size.x)
			{
				if (collides2d(hitbox, currentHitbox))
				{
					if (!addPassenger((mob*)possiblyCollidingEntity))
					{
						//minecart full
						return;
					}
				}
			}
		}
	}
}
void ridableEntity::tick()
{
	entity::tick();
	for (size_t i = 0; i < seats.size(); i++)
	{
		if (seats[i])
		{
			//search passengers
			entity* ridingEntity = dimensionIn->findUUID(newPosition, 0x8 + mobSizeMargin, seats[i]);
			if (ridingEntity)
			{
				if (((mob*)ridingEntity)->UUIDRidingOn == identifier)
				{
					ridingEntity->newPosition = newPosition + getRelativeSeatPosition(i);
					ridingEntity->speed = speed;
				}
				else
				{
					seats[i] = uuid();
				}
			}
			else
			{
				seats[i] = uuid();
			}
		}
	}
}