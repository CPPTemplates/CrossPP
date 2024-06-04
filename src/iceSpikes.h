#pragma once
#include "overWorldBiomeGenerator.h"
struct iceSpikes : public overWorldBiomeGenerator
{
	iceSpikes();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};