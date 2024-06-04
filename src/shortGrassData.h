#pragma once
#include "attachedBlockPollenData.h"
struct shortGrassData : attachedBlockPollenData
{
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
};