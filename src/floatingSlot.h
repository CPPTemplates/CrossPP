#pragma once
#include "entity.h"
#include "itemStack.h"
struct floatingSlot : public entity
{
	itemStack stack;
	floatingSlot(dimension *dimensionIn, cvec2 &position);
	// the amount of ticks since this floating slot is ready to be picked up
	// can also be negative. then it is still waiting until it can be picked up
	// it will be able to be picked up in half a second
	int ticksAvailable = ticksPerRealLifeSecond / -2;
	virtual void tick() override;
	virtual void render(const gameRenderData &targetData) const override;
	virtual ~floatingSlot() override;
	virtual void serializeValue(nbtSerializer &s) override;
};