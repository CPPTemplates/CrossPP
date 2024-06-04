#pragma once
#include "mob.h"
struct passiveMob : virtual mob
{
	passiveMob(dimension* dimensionIn, cvec2& position, const entityID& entityType);
};