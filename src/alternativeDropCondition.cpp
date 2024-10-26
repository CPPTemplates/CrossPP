#include "alternativeDropCondition.h"
bool alternativeDropCondition::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	for (dropCondition* conditionToCheck : conditionsToCheck)
	{
		if (conditionToCheck->checkCondition(data, randomToUse))
		{
			return true;
		}
	}
	return false;
}

alternativeDropCondition::~alternativeDropCondition()
{
	for (dropCondition* const& condition : conditionsToCheck)
	{
		delete condition;
	}
}

