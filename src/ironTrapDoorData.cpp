#include "ironTrapDoorData.h"
#include "tickableBlockContainer.h"
bool ironTrapDoorData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	containerIn->openDoorType(position, containerIn->getPowerLevel(position) > 0);
	return false;
}