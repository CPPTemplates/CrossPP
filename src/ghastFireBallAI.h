#include "globalFunctions.h"
#include "fireBallAI.h"
#pragma once
constexpr fp ghastSightRange = 0x20;
constexpr int ghastFireballCoolDown = 60;
constexpr int warningTick = 10;
struct ghastFireBallAI : fireBallAI
{
	int fireBallTicks = 0;
	virtual bool shouldExecute() override;
	virtual bool continueExecuting() override;
	virtual void updateTask() override;
	ghastFireBallAI(entity* connectedEntity) : fireBallAI(connectedEntity, entityID::ghast_fireball, ghastSightRange) {}
	virtual void serializeValue(nbtSerializer& s) override;
};