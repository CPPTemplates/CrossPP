#pragma once
#include "facingData.h"
#include "openData.h"
struct fenceGateData : virtual facingData, virtual openData
{
	fenceGateData(const directionID& directionFacing = directionID::negativeX, cbool& isOpen = false) :facingData(directionFacing), openData(isOpen) {}
	virtual void serializeValue(nbtSerializer& s) override;
};