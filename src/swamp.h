#pragma once
#include "overWorldBiomeGenerator.h"
struct swamp :public overWorldBiomeGenerator
{
	swamp();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};