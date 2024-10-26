#pragma once
#include "fireBall.h"
struct smallFireBall : public fireBall
{
	smallFireBall(dimension* dimensionIn, cvec2& position);
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};