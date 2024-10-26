#pragma once
#include "overWorldBiomeGenerator.h"
struct snowyTundra :public overWorldBiomeGenerator
{
	snowyTundra();
	virtual void  attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};