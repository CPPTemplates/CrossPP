#pragma once
#include "EntityAI.h"
constexpr int timeBetweenJumps = 9;
constexpr int ticksBetweenJumpsToFly = timeBetweenJumps - 3;
struct playerControlledAI final : entityAI
{
	bool wasJumping = false;
	int ticksSinceStoppedJumping = ticksBetweenJumpsToFly + 1;
	void execute() final;

	virtual void serializeValue(nbtSerializer& s) override;

	playerControlledAI(entity* const& connectedEntity) : entityAI(connectedEntity) {}
};