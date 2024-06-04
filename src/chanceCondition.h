#pragma once
#include "dropCondition.h"
struct chanceCondition :public dropCondition
{
	fp chance = 0;
	chanceCondition(cfp& chance = 1) :chance(chance) {}
	virtual bool checkCondition(const dropData& data, std::mt19937& randomToUse) const override;
};