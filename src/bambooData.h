#pragma once
#include "ageData.h"
struct bambooData : ageData
{
	int leavesSize = 0;
	int stalkThickness = 0;
	virtual void serializeValue(nbtSerializer& s) override;
};