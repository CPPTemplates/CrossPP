#pragma once
#include "attackOnCollideAI.h"
struct explodeAI : attackOnCollideAI
{
	explodeAI(entity* const& connectedEntity, const entityID& entityTypeToAttack, cfp& proximity = standardDetectionRange) :attackOnCollideAI(connectedEntity, entityTypeToAttack, 0x4) {}
	virtual void startExecuting() override;
	virtual bool continueExecuting() override;
	virtual void updateTask() override;
	virtual void resetTask() override;
};