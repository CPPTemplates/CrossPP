#include "taskAI.h"
bool taskAI::shouldExecute()
{
	return true;
}

bool taskAI::continueExecuting()
{
	return shouldExecute();
}

void taskAI::startExecuting()
{
	updateTask();
}

void taskAI::resetTask()
{
}

void taskAI::updateTask()
{
}

void taskAI::execute()
{
}

void taskAI::serializeValue(nbtSerializer& s)
{
}