#pragma once
#include "taskAI.h"
#include "attackOnCollideAI.h"
struct fireBallAI : attackOnCollideAI
{
	entityID fireBallType = (entityID)0;
	void shootFireBall(const entity* shootFireBallAt) const;
	fireBallAI(entity* connectedEntity, const entityID& fireBallType, cfp& sightRange) : attackOnCollideAI(connectedEntity, entityID::human, sightRange), fireBallType(fireBallType) {}
	virtual void serializeValue(nbtSerializer& s) override;
};