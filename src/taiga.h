#pragma once
#include "overWorldBiomeGenerator.h"
struct taiga : public overWorldBiomeGenerator
{
	layerNoiseSimplex* carrotNoise = nullptr;
	layerNoiseSimplex* pumpkinNoise = nullptr;
	taiga();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
	virtual ~taiga() override;
};