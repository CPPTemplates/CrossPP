#pragma once
#include "math/LayerNoiseSimplex.h"
#include "biomeGenerator.h"
struct overWorldBiomeGenerator :biomeGenerator
{
	overWorldBiomeGenerator(const biomeID& identifier);
	layerNoiseSimplex* elevationNoise = nullptr;
	virtual fp getHeight(cint& x) const;
	virtual ~overWorldBiomeGenerator() override;
	static bool shouldPlaceSugarCane(cveci2& pos, std::mt19937& randomToUse);
};