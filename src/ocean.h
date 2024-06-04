#pragma once
#include "overWorldBiomeGenerator.h"
struct ocean : public overWorldBiomeGenerator
{
	layerNoiseSimplex* kelpNoise = nullptr;
	ocean();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};