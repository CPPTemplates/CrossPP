#pragma once
#include "mob.h"
//keep this at first in the derived struct list!
struct fourLeggedMob : virtual mob
{
	fourLeggedMob(dimension* dimensionIn, cvec2& position, const entityID& entityType);

	bodyPart2D* backLeftLeg = nullptr;
	bodyPart2D* backRightLeg = nullptr;
	bodyPart2D* frontLeftLeg = nullptr;
	bodyPart2D* frontRightLeg = nullptr;

	virtual void updateBodyParts() const override;
	void connectBodyParts();
};