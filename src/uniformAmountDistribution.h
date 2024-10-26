#pragma once
#include "amountDistribution.h"
struct uniformAmountDistribution : public amountDistribution
{
	int minimum = 0, maximum = 0;
	virtual int getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const override;
	uniformAmountDistribution(cint& minimum = 1, cint& maximum = 1) : minimum(minimum), maximum(maximum) {}
};