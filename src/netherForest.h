#pragma once
#include "netherBiomeGenerator.h"
struct netherForest : public netherBiomeGenerator
{
	netherForest(const biomeID& identifier) : netherBiomeGenerator(identifier) {}
	virtual void attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const override;
};