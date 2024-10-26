#include "constants.h"
#include "world.h"
#include "include/filesystem/textfile.h"
fp getPowerStrength(const powerLevel &level)
{
	constexpr fp multiplier = powerStrengthStepCount / maxPowerLevel;

	return pow(2, (level * multiplier) - powerStrengthStepCount);
}