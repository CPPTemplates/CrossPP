#pragma once
#include "facingData.h"
#include "attachedBlockData.h"
struct inputData : facingData, attachedBlockData
{
	powerLevel lastPowerLevelBehind = 0;
	powerLevel lastPowerLevelBelow = 0;
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeValue(nbtSerializer& s) override;
	inputData(const directionID& directionFacing = standardSideFacingBlockDirection, const powerLevel& lastPowerLevelBehind = 0, const powerLevel& lastPowerLevelBelow = 0) : facingData(directionFacing), attachedBlockData(), lastPowerLevelBehind(lastPowerLevelBehind), lastPowerLevelBelow(lastPowerLevelBelow) {}
};