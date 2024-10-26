#pragma once
#include "EntityAI.h"
#include "taskAI.h"
//a list of tasks. task 0 is the most important
struct taskList : entityAI
{
	int executingTaskIndex = -1;
	std::vector<taskAI*> tasks;
	void addTask(cint& priority, taskAI* task)
	{
		tasks.insert(tasks.begin() + priority, task);
	}
	taskList(entity* connectedEntity = nullptr, std::vector<taskAI*> tasks = {}) : entityAI(connectedEntity), tasks(tasks) {}

	void execute() final;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual ~taskList();
};