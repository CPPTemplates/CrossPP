#include "conditionDrop.h"
#include <vector>
#include <random>
#include "constants.h"
#include "itemStack.h"
inline std::vector<itemStack> conditionDrop::roll(const dropData& data, std::mt19937& randomToUse) const
{
	return conditionToCheck->checkCondition(data, randomToUse) ? rollWhenTrue->roll(data, randomToUse) : std::vector<itemStack>();
}

conditionDrop::~conditionDrop()
{
	delete rollWhenTrue;
	delete conditionToCheck;
}