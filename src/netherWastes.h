#pragma once
#include "math/LayerNoiseSimplex.h"
#include "netherBiomeGenerator.h"

struct netherWastes : public netherBiomeGenerator
{
	layerNoiseSimplex* fireNoise = nullptr;
	netherWastes();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};