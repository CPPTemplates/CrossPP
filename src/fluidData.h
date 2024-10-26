#pragma once
#include "blockData.h"
#include "blockContainer.h"
struct fluidData : blockData
{
	fluidLevel currentFluidLevel = 0;//the water level
	//fp nextWaterLevel = 0;
	fluidData(const fluidLevel& currentFluidLevel = maxFluidLevel) :currentFluidLevel(currentFluidLevel) { }
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;

	//returns 0 if there is none of that fluid there, else the level
	static fluidLevel getFluidLevel(blockContainer* containerIn, cveci2& position, const blockID& fluid);
	virtual void serializeValue(nbtSerializer& s) override;
};