#pragma once
#include "netherBiomeGenerator.h"
struct soulSandValley : public netherBiomeGenerator
{
	soulSandValley();
	virtual void attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
	virtual void attemptgenerateCeilingStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};