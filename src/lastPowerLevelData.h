#pragma once
#include "blockData.h"
struct lastPowerLevelData : virtual blockData
{
	powerLevel lastPowerLevel = 0;
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeValue(nbtSerializer& s) override;
	lastPowerLevelData(const powerLevel& lastPowerLevel = 0) : lastPowerLevel(lastPowerLevel) {}
};