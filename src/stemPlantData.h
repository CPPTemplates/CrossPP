#pragma once
#include "cropData.h"

struct stemPlantData : cropData
{
	veci2 relativeFruitPosition = veci2();
	stemPlantData(cint& age = 0, cveci2& relativeFruitPosition = cveci2()) : cropData(age), relativeFruitPosition(relativeFruitPosition) {}
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeValue(nbtSerializer& s) override;
	void attemptToGrowFruit(tickableBlockContainer* containerIn, cveci2& position);
};