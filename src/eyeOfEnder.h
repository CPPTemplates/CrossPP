#pragma once
#include "throwable.h"
struct eyeOfEnder : public throwable
{
	int ticksToFloat = 0x50;
	vec2 strongholdPosition = vec2();
	eyeOfEnder(dimension* dimensionIn, cvec2& position);
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
	virtual void tick() override;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual fp getGravityForce() const override;
};