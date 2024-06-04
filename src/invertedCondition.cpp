#include "invertedCondition.h"
bool invertedCondition::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	return !conditionToInvert->checkCondition(data, randomToUse);
}

invertedCondition::~invertedCondition()
{
	delete conditionToInvert;
}