#pragma once
#include "taskAI.h"
struct loveModeAI : taskAI
{
	uuid partnerFound = uuid();
	virtual bool shouldExecute() override;
	//virtual void updateTask();
	loveModeAI(entity* const& connectedEntity) : taskAI(connectedEntity) {}
	virtual void serializeValue(nbtSerializer& s) override;
};