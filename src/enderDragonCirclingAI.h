#pragma once
#include "taskAI.h"
struct enderDragonCirclingAI : taskAI
{
	vec2 targetPoint = cvec2();
	enderDragonCirclingAI(entity* const& connectedEntity) :taskAI(connectedEntity) {}
	void startExecuting() override;
	void updateTask() override;
	void chooseTargetPoint();
	virtual void serializeValue(nbtSerializer& s) override;
};