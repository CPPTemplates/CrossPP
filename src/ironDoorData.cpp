#include "ironDoorData.h"
#include "tickableBlockContainer.h"
bool ironDoorData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (isPart0)
	{
		containerIn->openDoorType(position, containerIn->getPowerLevel(position) > 0);
	}
	return false;
}