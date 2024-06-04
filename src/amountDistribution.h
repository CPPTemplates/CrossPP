#pragma once
#include <random>
//#include "constants.h"
#include "interface/idestructable.h"
#include "dropData.h"
struct jsonContainer;
struct amountDistribution : IDestructable
{
	virtual int getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const = 0;
};
amountDistribution* parseMinimumAndMaximum(const jsonContainer& container);