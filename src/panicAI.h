#pragma once
#include "taskAI.h"
#include "math/uuid.h"
struct panicAI : taskAI
{
	//negative: looking back, positive: time since looking back
	int ticksSinceLookingBack = 0;

	uuid uuidFleeingFrom = uuid();
	//virtual void startExecuting() override;
	//run from attacker
	virtual bool shouldExecute() override;
	virtual bool continueExecuting() override;
	virtual void updateTask() override;
	virtual void resetTask() override;
	panicAI(entity* connectedEntity) :taskAI(connectedEntity) {}
	virtual void serializeValue(nbtSerializer& s) override;
};