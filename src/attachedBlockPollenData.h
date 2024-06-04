#pragma once
#include "pollenData.h"
#include "attachedBlockData.h"
struct attachedBlockPollenData : virtual pollenData, attachedBlockData
{
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override
	{
		return attachedBlockData::tick(containerIn, position);
	}
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override
	{
		return pollenData::randomTick(containerIn, position);
	}
	//using attachedBlockData::tick;  //doesn't work
	//using pollenData::randomTick;
	attachedBlockPollenData() : pollenData(), attachedBlockData() {}
};
