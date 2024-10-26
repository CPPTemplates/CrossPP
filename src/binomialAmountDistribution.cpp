#include "binomialAmountDistribution.h"
int binomialAmountDistribution::getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const
{
	std::binomial_distribution<int> dis = std::binomial_distribution<int>(n, p);
	return dis(randomToUse);
}