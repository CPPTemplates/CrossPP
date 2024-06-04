#pragma once
#include "amountDistribution.h"
struct constantAmountDistribution : public amountDistribution
{
	int amount = 1;
	virtual int getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const override;
	constantAmountDistribution(cint& amount = 1) :amount(amount) {}
};