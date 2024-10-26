#include "world.h"
lightLevel getVisibleSunLightLevel(const lightLevel& realSunLightLevel)
{
	return math::minimum((lightLevel)timeToLightLevel.getValue(currentWorld->getTimeOfDay()), realSunLightLevel);
}