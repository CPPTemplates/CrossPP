#pragma once
#include "itemTag.h"
#include "statusEffect.h"
struct potionData :itemTag
{
	std::vector<statusEffect> effectsToAdd;
	virtual void serializeValue(nbtSerializer& s) override;
	potionData(const std::vector<statusEffect>& effectsToAdd = std::vector<statusEffect>()) :effectsToAdd(effectsToAdd) {}
};