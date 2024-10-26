#pragma once
#include "attachedBlockData.h"
struct sugarCaneData :attachedBlockData
{
	sugarCaneData();
	void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
	bool attached(tickableBlockContainer* containerIn, cveci2& position) const override;
};