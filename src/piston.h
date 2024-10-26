#pragma once
#include "entity.h"
struct movableBlockContainer;
struct piston : public entity
{
	veci2 connectedPistonPosition = cveci2();
	movableBlockContainer* movingBlocks = nullptr;

	piston(dimension* dimensionIn, cvec2& position);
	virtual void tick() override;
	virtual void render(const gameRenderData& targetData) const override;

	virtual ~piston() override;
	virtual fp getGravityForce() const override;
	virtual void serializeValue(nbtSerializer& s) override;
};