#include "amountDrop.h"
std::vector<itemStack> amountDrop::roll(const dropData& data, std::mt19937& randomToUse) const
{
	std::vector<itemStack> stacksToIncrease = dropsToIncrease->roll(data, randomToUse);
	std::vector<itemStack> increasedStacks = std::vector<itemStack>();
	for (size_t i = 0; i < stacksToIncrease.size(); i++)
	{
		stacksToIncrease[i].count = distribution->getAmount(stacksToIncrease[i].count, data, randomToUse);
		if (stacksToIncrease[i].count > 0)
		{
			increasedStacks.push_back(stacksToIncrease[i]);
		}
		else
		{

		}
	}
	return increasedStacks;
}

amountDrop::~amountDrop()
{
	delete dropsToIncrease;
	delete distribution;
}