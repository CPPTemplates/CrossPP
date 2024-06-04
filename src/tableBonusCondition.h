#pragma once
#include "enchantmentID.h"
#include "dropCondition.h"
struct tableBonusCondition : public dropCondition
{
	enchantmentID enchantmentRequired = enchantmentID();
	std::vector<fp> chanceTable = std::vector<fp>();

	virtual bool checkCondition(const dropData& data, std::mt19937& randomToUse) const override;
};