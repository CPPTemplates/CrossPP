#include "humanoid.h"
#pragma once
struct zombifiedPiglin : public humanoid
{
	zombifiedPiglin(dimension* dimensionIn, cvec2& position);
	virtual void render(const gameRenderData& targetData) const override;
	virtual void serializeValue(nbtSerializer& s) override;
};