#pragma once
#include "fluidData.h"
struct spongeData : fluidData
{
	spongeData(const fluidLevel& currentFluidLevel = 0) :fluidData(currentFluidLevel) { }
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
};