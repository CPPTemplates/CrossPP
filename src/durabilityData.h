#pragma once
#include "itemTag.h"
struct durabilityData : itemTag
{
	fp durability = 1;
	virtual void serializeValue(nbtSerializer& s) override;
};