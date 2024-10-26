#pragma once
#include "nbt/nbtSerializable.h"
#include "statusEffectID.h"
struct statusEffect :nbtSerializable
{
	statusEffectID identifier = statusEffectID();
	int ticksDuration = 0;
	int potency = 1;
	statusEffect(const statusEffectID& identifier = statusEffectID(), cint& ticksDuration = 0, cint& potency = 1) :identifier(identifier), ticksDuration(ticksDuration), potency(potency) {}
	virtual void serializeValue(nbtSerializer& s) override;
};