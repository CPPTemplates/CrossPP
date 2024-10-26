#include "collisionData.h"
#include <math/collisions.h>
#include "constants.h"
#include "include/optimization/handleError.h"
void collisionData::evaluate(crectangle2& hitboxToTest, cvec2& hitboxSpeed, cfp& maxDuration)
{
	collisionTime = collideTime2d(hitboxToTest, hitboxCollidingWith, (hitboxSpeed - speed) * maxDuration, axisCollided) * maxDuration;
	if (std::isnan(collisionTime))
	{
		handleError(std::wstring(L"error"));
	}
}