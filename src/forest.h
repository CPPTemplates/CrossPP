#pragma once
#include "overWorldBiomeGenerator.h"
struct forest :public overWorldBiomeGenerator
{
	forest();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};