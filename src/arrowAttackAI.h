#pragma once
#include "attackOnCollideAI.h"
struct arrowAttackAI : attackOnCollideAI
{
	virtual void updateTask() override;
	virtual bool shouldExecute() override;
	arrowAttackAI(entity* const& connectedEntity, const entityID& entityTypeToAttack, cfp& proximity = standardDetectionRange) :attackOnCollideAI(connectedEntity, entityTypeToAttack, proximity) {}
};