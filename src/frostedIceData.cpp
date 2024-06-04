#include "frostedIceData.h"
#include "tickableBlockContainer.h"
#include "math/random/random.h"
bool frostedIceData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	const lightLevel& level = containerIn->getVisibleLightLevel(position);

	if (level > glowInTheDarkLightLevel)
	{
		if (randChance(currentRandom, ticksPerRealLifeSecond))
		{
			age++;
			if (age >= frostedIceMeltTreshold)
			{
				containerIn->setBlockID(position, blockID::water);
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}