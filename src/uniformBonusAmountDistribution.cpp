#include "uniformBonusAmountDistribution.h"
#include "dropData.h"
#include "itemStack.h"
#include "include/math/random/random.h"
int uniformBonusAmountDistribution::getAmount(cint& amountToIncrease, const dropData& data, std::mt19937& randomToUse) const
{
	return data.toolUsed ?
		amountToIncrease + roundRandom(randomToUse, randFp(randomToUse, data.toolUsed->getEnchantmentLevel(enchantmentToUse) * bonusMultiplier)) : amountToIncrease;
}