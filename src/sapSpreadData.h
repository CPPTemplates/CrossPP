#pragma once
#include "blockData.h"
constexpr int maxSapLevel = 0x10;//maximum = 16 blocks
struct sapSpreadData : virtual blockData
{
	int sapLevel;
	sapSpreadData(cint& sapLevel = maxSapLevel) :sapLevel(sapLevel) {}
	virtual void serializeValue(nbtSerializer& s) override;
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
};