#pragma once
#include "facingData.h"
#include "upsideDownData.h"
struct stairsData : facingData, upsideDownData
{
	stairsData(const directionID& directionFacing = directionID::negativeX, cbool& upsideDown = true) :
		facingData(directionFacing), upsideDownData(upsideDown) {}
	virtual void serializeValue(nbtSerializer& s) override;
};