#pragma once
#include "doubleBlockData.h"
#include "facingData.h"
#include "openData.h"

struct doorData : virtual doubleBlockData, virtual facingData, virtual openData
{
	doorData(cbool& isBottomPart = false) :doubleBlockData(isBottomPart), facingData(directionID::negativeX), openData(false) {}
	virtual void serializeValue(nbtSerializer& s) override;
};