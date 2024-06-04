#pragma once
#include "fireBall.h"
struct ghastFireBall : public fireBall
{
	ghastFireBall(dimension* dimensionIn, cvec2& position);
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};