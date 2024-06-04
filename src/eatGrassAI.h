#pragma once
#include "taskAI.h"
struct eatGrassAI : taskAI
{
	virtual bool shouldExecute() override;
	virtual bool continueExecuting() override;
	virtual void startExecuting() override;
	eatGrassAI(entity* const& connectedEntity) :taskAI(connectedEntity) {}
};