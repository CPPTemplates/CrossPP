#pragma once
#include "taskAI.h"
struct enderDragonPortalFlyingAI : taskAI
{
	enderDragonPortalFlyingAI(entity* const& connectedEntity) :taskAI(connectedEntity) {}
	bool shouldExecute() override;
	void updateTask() override;
};