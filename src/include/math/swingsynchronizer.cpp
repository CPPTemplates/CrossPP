#include "swingsynchronizer.h"
#include "mathFunctions.h"
#include <cmath>

fp swingSynchronizer::getSwingAngle(cfp& timePoint) const
{
	return math::mapValue(sin(((timePoint - offset) / swingDuration) * math::PI2), (fp)-1, (fp)1, minAngle, maxAngle);
}

bool swingSynchronizer::maximumBetween(cfp& timePoint0, cfp& timePoint1) const noexcept
{
	return floor((timePoint0 - offset) / swingDuration) != floor((timePoint1 - offset) / swingDuration);
}