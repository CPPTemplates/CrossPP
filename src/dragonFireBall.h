#pragma once
#include "fireBall.h"
struct dragonFireBall : public fireBall
{
	dragonFireBall(dimension* dimensionIn, cvec2& position);
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};