#include "redstoneLampData.h"
#include "tickableBlockContainer.h"
bool redstoneLampData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	const powerLevel oldPowerLevel = lastPowerLevel;
	lastPowerLevelData::tick(containerIn, position);
	if (lastPowerLevel != oldPowerLevel)
	{
		containerIn->addUpdatePosition(position);
	}
	return false;
}