#pragma once
#include "taskAI.h"
struct teleportToAttachAI : taskAI
{
	bool shouldExecute() override;
	void updateTask() override;
	teleportToAttachAI(entity* const& connectedEntity = nullptr) : taskAI(connectedEntity) {}
};