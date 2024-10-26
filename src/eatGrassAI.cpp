#include "eatGrassAI.h"
#include "sheep.h"
bool eatGrassAI::shouldExecute()
{
	return randChance(currentRandom, 0x1000);
}

bool eatGrassAI::continueExecuting()
{
	return false;
}

void eatGrassAI::startExecuting()
{
	dynamic_cast<sheep*>(connectedEntity)->hasWool = true;
}