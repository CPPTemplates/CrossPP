#pragma once
#include "taskAI.h"
struct boidsAI : taskAI
{
	virtual bool shouldExecute() override;
	virtual void updateTask() override;
	boidsAI(entity* connectedEntity);
};