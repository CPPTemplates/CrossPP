#pragma once
#include "ridableEntity.h"
#include "math/bodypart2d.h"
struct hollowRidableEntity : public ridableEntity
{
	bodyPart2D* mainBodyPart = nullptr;
	virtual fp getWeight() const final;
	virtual fp getWeightPerCubicMeter() const = 0;
	virtual fp getHollowVolume() const = 0;
	hollowRidableEntity(dimension* dimensionIn, cvec2& position, const entityID& entityType, cint& seatCount = 1) :
		ridableEntity(dimensionIn, position, entityType, seatCount) {}
};