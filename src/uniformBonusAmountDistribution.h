#pragma once
#include "bonusAmountDistribution.h"
struct uniformBonusAmountDistribution : bonusAmountDistribution
{
	fp bonusMultiplier = 0;
	virtual int getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const override;
};