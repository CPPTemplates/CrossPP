#pragma once
#include "targetAI.h"
struct attackOnCollideAI : targetAI
{
	entityID entityTypeToAttack;
	entity* findNearestAttackableTarget() const;
	virtual bool shouldExecute() override;
	virtual bool continueExecuting() override;
	attackOnCollideAI(entity* const& connectedEntity, const entityID& entityTypeToAttack, cfp& proximity = standardDetectionRange) :targetAI(connectedEntity, proximity), entityTypeToAttack(entityTypeToAttack) {}
	virtual void serializeValue(nbtSerializer& s) override;
};