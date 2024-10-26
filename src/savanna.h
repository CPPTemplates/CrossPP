#pragma once
#include "overWorldBiomeGenerator.h"
struct savanna : public overWorldBiomeGenerator
{
	savanna();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};