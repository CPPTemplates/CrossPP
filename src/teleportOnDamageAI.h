#pragma once
#include "taskAI.h"
struct teleportOnDamageAI : taskAI
{
	int lastDamageTick = -1;
	virtual bool shouldExecute() override;
	virtual bool continueExecuting() override;
	virtual void updateTask() override;
	teleportOnDamageAI(entity* const& connectedEntity) :taskAI(connectedEntity) {}
};