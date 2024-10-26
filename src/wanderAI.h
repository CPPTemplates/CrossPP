#pragma once
#include "taskAI.h"
struct wanderAI : taskAI
{
	fp targetX = 0;
	virtual bool continueExecuting() override;
	virtual void startExecuting() override;
	virtual void updateTask() override;
	virtual void resetTask() override;
	wanderAI(entity* connectedEntity);
	virtual void serializeValue(nbtSerializer& s) override;
};