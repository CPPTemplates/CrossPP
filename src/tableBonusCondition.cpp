#include "tableBonusCondition.h"
#include "dropData.h"
#include "itemStack.h"
#include "include/math/random/random.h"
bool tableBonusCondition::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	cint level = data.toolUsed ? math::minimum(data.toolUsed->getEnchantmentLevel(enchantmentRequired), (int)(chanceTable.size() - 1)) : 0;
	return randFp(randomToUse) < chanceTable[level];
}
