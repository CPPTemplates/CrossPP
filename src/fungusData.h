#pragma once
#include "pollenData.h"
#include "saplingData.h"
struct fungusData : pollenData, saplingData
{
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override
	{
		return saplingData::tick(containerIn, position);
	}
};