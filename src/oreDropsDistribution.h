#pragma once
#include "bonusAmountDistribution.h"
struct oreDropsDistribution : bonusAmountDistribution
{
	virtual int getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const override;
};