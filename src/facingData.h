#pragma once
#include "blockData.h"
//the block textures have to face to negative x by default
struct facingData : virtual blockData
{
	directionID directionFacing = directionID::positiveY;
	facingData(const directionID& directionFacing = directionID::positiveY) :directionFacing(directionFacing) {}
	virtual void serializeValue(nbtSerializer& s) override;
};