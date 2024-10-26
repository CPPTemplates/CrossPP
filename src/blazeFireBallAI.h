#pragma once
#include "fireBallAI.h"
cfp fireBallRange = 0x30;
struct blazeFireBallAI : fireBallAI
{
	int fireBallCoolDown = 0;
	int fireBallsLeft = 0;
	virtual bool shouldExecute() override;
	virtual bool continueExecuting() override;
	blazeFireBallAI(entity* connectedEntity) : fireBallAI(connectedEntity, entityID::small_Fireball, fireBallRange) {}
	virtual void updateTask() override;
	virtual void serializeValue(nbtSerializer& s) override;
};