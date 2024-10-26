#include "oreDropsDistribution.h"
#include "dropData.h"
#include "itemStack.h"
#include "math/random/random.h"
int oreDropsDistribution::getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const
{
	cint level = data.toolUsed ? data.toolUsed->getEnchantmentLevel(enchantmentToUse) : 0;

	return amountToIncrease + math::maximum(rand(randomToUse, level + 1) - 1, 0);
}