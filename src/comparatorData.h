#pragma once
#include "inputData.h"
struct comparatorData : inputData
{
	bool substractionMode = false;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
};