#include "explosionDecayDrop.h"
#include "dropData.h"
#include "math/random/random.h"
std::vector<itemStack> explosionDecayDrop::roll(const dropData& data, std::mt19937& randomToUse) const
{
	std::vector<itemStack> stacks = dropsToDecay->roll(data, randomToUse);
	if (data.explosionPower > 1)
	{
		std::vector<itemStack> remainingStacks = std::vector<itemStack>();
		for (itemStack& stack : stacks)
		{
			int remainingAmount = 0;
			for (int i = 0; i < stack.count; i++)
			{
				if (randChance(randomToUse, data.explosionPower))
				{
					remainingAmount++;
				}
			}
			if (remainingAmount)
			{
				stack.count = remainingAmount;
				remainingStacks.push_back(stack);
			}
			else
			{

			}
		}
		return remainingStacks;
	}
	else
	{
		return stacks;
	}
}

explosionDecayDrop::~explosionDecayDrop()
{
	delete dropsToDecay;
}