#pragma once
#include "dropCondition.h"
struct invertedCondition :public dropCondition
{
	dropCondition* conditionToInvert = nullptr;
	virtual bool checkCondition(const dropData& data, std::mt19937& randomToUse) const override;
	virtual ~invertedCondition() override;
};