#include "chanceCondition.h"
#include "math/random/random.h"
bool chanceCondition::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	return randFp(randomToUse) < chance;
}