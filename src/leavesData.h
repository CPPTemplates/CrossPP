#pragma once
#include "sapSpreadData.h"
#include "pollenData.h"
struct leavesData : sapSpreadData, pollenData
{
	virtual void serializeValue(nbtSerializer& s) override {
		sapSpreadData::serializeValue(s);
	}
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override {
		pollenData::randomTick(containerIn, position);
		sapSpreadData::randomTick(containerIn, position);
	}
};