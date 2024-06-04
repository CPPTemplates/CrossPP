#include "rollDrop.h"
#include <random>
#include <vector>
#include "constants.h"
#include "globalFunctions.h"
#include "itemStack.h"
inline std::vector<itemStack> rollDrop::roll(const dropData& data, std::mt19937& randomToUse) const
{
	cint rollCount = distribution->getAmount(0, data, randomToUse);

	std::vector<itemStack> rolledItems = std::vector<itemStack>();

	for (int i = 0; i < rollCount; i++)
	{
		const std::vector<itemStack> currentRoll = toRoll->roll(data, randomToUse);
		rolledItems.insert(rolledItems.end(), currentRoll.begin(), currentRoll.end());
	}
	return rolledItems;
}

rollDrop::~rollDrop()
{
	delete distribution;
	delete toRoll;
}