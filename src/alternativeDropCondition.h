#pragma once
#include "dropCondition.h"
//it will return the drop if one of the conditions succeds
struct alternativeDropCondition : public dropCondition
{
	std::vector<dropCondition*> conditionsToCheck = std::vector<dropCondition*>();
	virtual bool checkCondition(const dropData& data, std::mt19937& randomToUse) const override;
	virtual ~alternativeDropCondition() override;
};