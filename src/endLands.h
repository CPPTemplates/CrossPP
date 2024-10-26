#pragma once
#include "endBiomeGenerator.h"
struct endLands final : public endBiomeGenerator
{
	endLands();
	virtual void attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};