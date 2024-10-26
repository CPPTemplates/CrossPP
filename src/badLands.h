#pragma once
#include "overWorldBiomeGenerator.h"
constexpr fp terracottaNoiseInfluence = 0x10;
struct badlands : public overWorldBiomeGenerator
{
	layerNoiseSimplex* terracottaNoise = nullptr;
	badlands();
	virtual void attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
	virtual ~badlands() override;
};