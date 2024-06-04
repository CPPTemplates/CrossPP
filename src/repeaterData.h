#pragma once
#include "inputData.h"
constexpr int delayArraySize = 0x6;
constexpr int delayArray[delayArraySize]
{
	0b1,0b10,0b100,0b1000, 0b10000, 0b100000
};
constexpr int delayedPowerLevelArraySize = delayArray[delayArraySize - 1];

struct repeaterData : inputData
{
	int delayArrayIndex = 0;//1, 2, 4 or 8
	powerLevel lockedPowerLevel = 0;

	powerLevel delayedPowerLevels[delayedPowerLevelArraySize]{};

	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeValue(nbtSerializer& s) override;
};