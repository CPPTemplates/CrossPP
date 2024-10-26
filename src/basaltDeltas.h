#pragma once
#include "math/LayerNoiseSimplex.h"
#include "netherBiomeGenerator.h"
struct basaltDeltas : public netherBiomeGenerator
{
	layerNoiseSimplex* basaltNoise = nullptr;
	basaltDeltas();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};