#pragma once
#include "enchantmentID.h"
#include "amountDistribution.h"
struct bonusAmountDistribution : amountDistribution
{
	enchantmentID enchantmentToUse = enchantmentID();
};