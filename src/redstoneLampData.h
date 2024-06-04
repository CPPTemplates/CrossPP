#pragma once
#include "lastPowerLevelData.h"
struct redstoneLampData : lastPowerLevelData
{
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
};