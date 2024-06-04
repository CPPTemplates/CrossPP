#pragma once
#include "gameRenderData.h"
#include "particleID.h"
#include "nbt/nbtSerializable.h"
#include "interface/idestructable.h"

struct particleBrush : nbtSerializable, IDestructable
{
	particleID particleType = particleID();
	particleBrush(const particleID& particleType) :particleType(particleType) {}
	virtual void render(crectangle2& rect, const gameRenderData& targetData) const = 0;
};