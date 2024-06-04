#pragma once
#include "overWorldBiomeGenerator.h"
struct mountains : public overWorldBiomeGenerator
{
	layerNoiseSimplex* coverageNoise = nullptr;
	mountains();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};