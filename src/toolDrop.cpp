#include "toolDrop.h"
#include "dropData.h"
#include "itemStack.h"
bool toolDrop::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	if (!data.toolUsed || (data.toolUsed->count == 0))
	{
		return false;
	}
	for (const enchantment& requiredEnchantment : requiredEnchantments)
	{
		if (data.toolUsed->getEnchantmentLevel(requiredEnchantment.identifier) < requiredEnchantment.power)
		{
			return false;
		}
	}

	if (requiredTool != itemID())
	{
		if (!data.toolUsed || data.toolUsed->stackItemID != requiredTool)
		{
			return false;
		}
	}
	return true;
}