#pragma once
#include "biomeGenerator.h"
struct netherBiomeGenerator : biomeGenerator
{
	netherBiomeGenerator(const biomeID& identifier);
	//pos.x: the x coordinate
	//pos.y: the exact height at which the roof is (30 means the ceiling is from 30 to ##)
	virtual void attemptgenerateCeilingStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const;
};