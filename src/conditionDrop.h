#pragma once
#include "lootTable.h"
#include "dropCondition.h"
struct conditionDrop : public lootTable
{
	lootTable* rollWhenTrue = nullptr;
	dropCondition* conditionToCheck = nullptr;
	conditionDrop(lootTable* rollWhenTrue = nullptr, dropCondition* conditionToCheck = nullptr) :rollWhenTrue(rollWhenTrue), conditionToCheck(conditionToCheck) {}
	inline std::vector<itemStack> roll(const dropData& data, std::mt19937& randomToUse) const override;
	virtual ~conditionDrop() override;
};