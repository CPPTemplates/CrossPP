#pragma once
#include "EntityAI.h"
struct taskAI : entityAI
{
	//returns true if this is the task to use
	virtual bool shouldExecute();
	virtual bool continueExecuting();
	virtual void startExecuting();
	virtual void resetTask();
	virtual void updateTask();

	virtual void execute() final;

	taskAI(entity* const& connectedEntity) :entityAI(connectedEntity) {}
	virtual void serializeValue(nbtSerializer& s) override;
};