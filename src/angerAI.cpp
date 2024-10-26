#include "angerAI.h"
#include "nbt/nbtSerializer.h"
bool angerAI::shouldExecute()
{
	return ticksAngry > 0 && targetAI::shouldExecute();
}

bool angerAI::continueExecuting()
{
	return shouldExecute();
}

void angerAI::serializeValue(nbtSerializer& s)
{
	targetAI::serializeValue(s);
	s.serializeValue(std::wstring(L"ticks angry"), ticksAngry);
}