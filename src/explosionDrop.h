#pragma once
#include "dropCondition.h"
struct explosionDrop : dropCondition
{
	virtual bool checkCondition(const dropData& data, std::mt19937& randomToUse) const override;
};