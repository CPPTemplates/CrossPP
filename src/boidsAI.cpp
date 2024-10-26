#include "boidsAI.h"
#include "dimension.h"
#include "mob.h"

constexpr fp codForce = 0.2;

bool boidsAI::shouldExecute()
{
	return true;
}

void boidsAI::updateTask()
{
	//http://www.vergenet.net/~conrad/boids/pseudocode.html


	vec2 centerOfMass = vec2();

	vec2 averageSpeed = vec2();

	size_t boidCount = 0;

	vec2 acceleration = vec2();

	constexpr fp desiredDistance = 1;

	const std::vector<entity*>& nearEntities = connectedEntity->dimensionIn->findNearEntities(connectedEntity->position, 0x10);
	for (const entity* const& e : nearEntities)
	{
		if (e->entityType == connectedEntity->entityType)
		{
			if (e->identifier != connectedEntity->identifier)
			{
				centerOfMass += e->position;

				boidCount++;

				cvec2& difference = e->position - connectedEntity->position;

				cfp& distanceSquared = difference.lengthSquared();

				if ((distanceSquared > 0) && (distanceSquared < math::squared(desiredDistance)))
				{
					acceleration -= difference.normalized() * 2;
				}

				averageSpeed += e->speed;
			}
		}
	}

	if (boidCount)
	{
		centerOfMass /= (fp)boidCount;
		averageSpeed /= (fp)boidCount;
		if (centerOfMass != connectedEntity->position)
		{
			acceleration += (centerOfMass - connectedEntity->position).normalized();
		}

		acceleration += (averageSpeed - connectedEntity->speed) * 0.125;

		mob* connectedMob = (mob*)connectedEntity;

		connectedMob->speed += acceleration * codForce;
		connectedMob->flipBodyToSpeedDirection();
	}
}

boidsAI::boidsAI(entity* connectedEntity) : taskAI(connectedEntity)
{
}