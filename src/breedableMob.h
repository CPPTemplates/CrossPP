#pragma once
#include "mob.h"
struct breedableMob : virtual mob
{
	breedableMob(dimension* dimensionIn, cvec2& position, const entityID& entityType) :
		mob(dimensionIn, position, entityType) {}
	int age = 0;
	virtual void tick() override;
	virtual void serializeValue(nbtSerializer& s) override;
};