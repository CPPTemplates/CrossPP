#include "array/arrayFunctions/arrayFunctions.h"
#include "incompatibleEnchantments.h"
bool canCombine(const enchantmentID& enchantment0, const enchantmentID& enchantment1)
{
	for (int incompatibleEnchantmentIndex = 0; incompatibleEnchantmentIndex < measureSize(incompatibleEnchantments); incompatibleEnchantmentIndex++)
	{
		std::vector<enchantmentID> checkVector = incompatibleEnchantments[incompatibleEnchantmentIndex];

		auto it0 = std::find(checkVector.begin(), checkVector.end(), enchantment0);
		auto it1 = std::find(checkVector.begin(), checkVector.end(), enchantment1);
		if (it0 != checkVector.end() && it1 != checkVector.end())
		{
			return false;
		}
	}
	return true;
}