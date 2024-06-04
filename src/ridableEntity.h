#pragma once
#include "entity.h"
struct ridableEntity : public entity
{
	std::vector<uuid> seats = std::vector<uuid>();
	virtual void tick() override;
	virtual vec2 getRelativeSeatPosition(csize_t& seatIndex) const = 0;
	bool addPassenger(mob* m);
	virtual void serializeValue(nbtSerializer& s) override;
	ridableEntity(dimension* dimensionIn, cvec2& position, const entityID& entityType, cint& seatCount = 1);

	//riding mobs exit automatically on death
	void addCollidingEntities();
};