#include "eyeOfEnder.h"
#include "itemDrop.h"
#include "dimension.h"
#include "soundList.h"
#include "structureID.h"
#include "nbt/nbtSerializer.h"
#include "math/random/random.h"
constexpr fp maxEyeVelocity = 10;
void eyeOfEnder::tick()
{
	if (ticksToFloat >= 0x20)
	{
		cfp dx = strongholdPosition.x - position.x;
		cfp distance = abs(dx);

		//nearer than this amount of blocks, the eye goes down
		cfp digDistance = 0x10;

		//float up if it is far, down if it is nearby
		cfp upwardsVelocity = math::maximum((fp)((1.0 - ((digDistance / distance) * 2.0)) * maxEyeVelocity), (fp)(-maxEyeVelocity));

		//float faster if it is farther away
		cfp sideVelocity = math::maximum((1.0 - (digDistance / distance)) * maxEyeVelocity, (fp)0.0);
		cfp signedSideVelocity = dx < 0 ? -sideVelocity : sideVelocity;
		speed += cvec2(signedSideVelocity, upwardsVelocity);
	}
	else
	{
		if (ticksToFloat >= 0x18)
		{
			//brake
			cfp brakeFriction = 0.9;
			speed *= brakeFriction;
		}
		else
		{
			if (ticksToFloat > 0)
			{
				//wobble in the air down, up, down
				if (ticksToFloat % 0x10 >= 0x8)
				{
					speed.y += maxEyeVelocity;
				}
				else
				{
					speed.y -= maxEyeVelocity;
				}
			}
			else
			{
				//break or drop
				despawn = true;
				if (!randChance(currentRandom, 5))
				{
					//drop as item
					itemDrop::dropLoot({ itemStack(itemID::ender_eye, 1) }, dimensionIn, position);
				}
				eyeOfEnderDropSound->playRandomSound(dimensionIn, position);
			}
		}
	}

	ticksToFloat--;
}

void eyeOfEnder::serializeValue(nbtSerializer& s)
{
	//the stronghold direction will be calculated
	s.serializeValue(std::wstring(L"ticks to float"), ticksToFloat);
}

fp eyeOfEnder::getGravityForce() const
{
	return 0;
}
eyeOfEnder::eyeOfEnder(dimension* dimensionIn, cvec2& position) : throwable(dimensionIn, position, entityID::ender_eye)
{
	//calculate distance to the nearest stronghold
	dimensionIn->locateStructure(structureID::stronghold, position, strongholdPosition);
}

void eyeOfEnder::onCollision(const std::vector<entity*>& collidingEntities)
{

}