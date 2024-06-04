#pragma once
#include "attachedBlockPollenData.h"
struct mushroomData : attachedBlockPollenData
{
	mushroomData() :attachedBlockPollenData() {}
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
};