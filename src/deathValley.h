#pragma once
#include "overWorldBiomeGenerator.h"
struct deathValley : public overWorldBiomeGenerator
{
	deathValley();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};