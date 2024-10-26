#pragma once
#include "facingData.h"
struct endPortalFrameData : facingData
{
	bool hasEye = false;
	endPortalFrameData(const directionID& directionFacing = directionID::negativeX, cbool& hasEye = false) :facingData(directionFacing), hasEye(hasEye) {}
	virtual void serializeValue(nbtSerializer& s) override;
};