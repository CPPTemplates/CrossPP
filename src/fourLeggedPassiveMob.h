#pragma once
#include "fourLeggedMob.h"
#include "passiveMob.h"
struct fourLeggedPassiveMob : public fourLeggedMob, public passiveMob
{
	fourLeggedPassiveMob(dimension* dimensionIn, cvec2& position, const entityID& entityType) : mob(dimensionIn, position, entityType), fourLeggedMob(dimensionIn, position, entityType), passiveMob(dimensionIn, position, entityType) {}

	virtual void updateBodyParts() const override {
		fourLeggedMob::updateBodyParts();
	}
};