#pragma once
#include "overWorldBiomeGenerator.h"
struct desert : public overWorldBiomeGenerator
{
	desert();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};