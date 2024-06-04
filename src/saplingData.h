#pragma once
#include "attachedBlockData.h"
struct saplingData : attachedBlockData
{
	saplingData();
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
};