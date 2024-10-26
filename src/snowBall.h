#pragma once
#include "throwable.h"
struct snowBall : public throwable
{
	snowBall(dimension* dimensionIn, cvec2& position);
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};