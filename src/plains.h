#pragma once
#include "overWorldBiomeGenerator.h"
struct plains :public overWorldBiomeGenerator
{
	layerNoiseSimplex* farmlandNoise = nullptr;
	plains();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
	virtual ~plains() override;
};