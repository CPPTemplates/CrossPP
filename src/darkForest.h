#pragma once
#include "overWorldBiomeGenerator.h"
struct darkForest : public overWorldBiomeGenerator
{
	darkForest();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};