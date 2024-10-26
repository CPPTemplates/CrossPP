#include "teleportToAttachAI.h"
#include "shulker.h"
bool teleportToAttachAI::shouldExecute()
{
	return !((shulker*)connectedEntity)->attached();
}

void teleportToAttachAI::updateTask()
{
	((shulker*)connectedEntity)->teleportToAttachableBlock();
}