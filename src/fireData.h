#pragma once
#include "ageData.h"
#include "attachedBlockData.h"

struct fireData : ageData, attachedBlockData
{
	fireData(cint& age = 0) : ageData(age) {}
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
	using attachedBlockData::tick;
	virtual void serializeValue(nbtSerializer& s) override;
};