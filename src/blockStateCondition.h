#pragma once
#include "array/wstringFunctions.h"
#include "dropCondition.h"
struct blockStateCondition : public dropCondition
{
	wstringContainer blockStateNames = wstringContainer();
	wstringContainer blockStateValues = wstringContainer();

	virtual bool checkCondition(const dropData& data, std::mt19937& randomToUse) const override;
};