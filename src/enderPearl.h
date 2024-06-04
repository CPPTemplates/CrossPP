#pragma once
#include "throwable.h"
struct enderPearl : public throwable
{
	//uuid uuidTeleporting = uuid();
	enderPearl(dimension* dimensionIn, cvec2& position);
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};