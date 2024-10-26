#pragma once
#include "facingData.h"
#include "upsideDownData.h"
#include "openData.h"
struct trapDoorData : virtual facingData, virtual upsideDownData, virtual openData
{
	trapDoorData(const directionID& directionFacing = directionID::negativeX, cbool& upsideDown = false, cbool& isOpen = false) :
		facingData(directionFacing), upsideDownData(upsideDown), openData(isOpen) {}
	virtual void serializeValue(nbtSerializer& s) override;
};