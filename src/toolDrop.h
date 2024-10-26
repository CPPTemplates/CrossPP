#pragma once
#include "enchantment.h"
#include "dropCondition.h"
struct toolDrop : public dropCondition
{
	std::vector<enchantment> requiredEnchantments = std::vector<enchantment>();
	itemID requiredTool = itemID();

	virtual bool checkCondition(const dropData& data, std::mt19937& randomToUse) const override;
};