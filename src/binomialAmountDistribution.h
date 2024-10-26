#pragma once
#include "amountDistribution.h"
struct binomialAmountDistribution : public amountDistribution
{
	int n = 0;
	fp p = 0;
	virtual int getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const override;
	binomialAmountDistribution(cint& n = 0, cfp& p = 0) :n(n), p(p) {}
};