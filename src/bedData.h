#pragma once
#include "doubleBlockData.h"
#include "facingData.h"
struct bedData :virtual doubleBlockData, virtual facingData
{
	bedData(cbool& isBottomPart = false) :doubleBlockData(isBottomPart), facingData(directionID::negativeX) {}
	virtual void serializeValue(nbtSerializer& s) override;
};
