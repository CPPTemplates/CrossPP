#pragma once
#include "bonusAmountDistribution.h"
struct binomialWithBonusCountDistribution : bonusAmountDistribution
{
	int extra = 0;
	fp probability = 0;
	virtual int getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const override;
};