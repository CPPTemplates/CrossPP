#pragma once
#include "itemTag.h"
#include "enchantmentID.h"
struct enchantment :itemTag
{
	enchantment() :identifier((enchantmentID)0), power(0) {}
	enchantment(const enchantmentID& identifier, cint& power) :identifier(identifier), power(power) {}
	enchantmentID identifier = (enchantmentID)0;
	int power = 0;
	virtual void serializeValue(nbtSerializer& s) override;
	std::wstring toWString() const;
};