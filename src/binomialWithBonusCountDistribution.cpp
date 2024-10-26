#include "binomialWithBonusCountDistribution.h"
#include "dropData.h"
#include "itemStack.h"
int binomialWithBonusCountDistribution::getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const
{
	std::binomial_distribution<int> dis = std::binomial_distribution<int>(data.toolUsed ? (data.toolUsed->getEnchantmentLevel(enchantmentToUse) + extra) : 0, probability);
	return amountToIncrease + dis(randomToUse);
}