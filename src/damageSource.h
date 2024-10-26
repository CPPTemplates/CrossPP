#pragma once
#include "interface/idestructable.h"
#include "damageType.h"
struct damageSource :IDestructable
{
	damageType type;
	int damageTick;//tick at wich the entity took damage since the start of the game
	damageSource(damageType type);
	virtual ~damageSource() override;
};