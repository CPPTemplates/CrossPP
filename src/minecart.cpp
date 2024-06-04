#include "idAnalysis.h"
#include "lootTable.h"
#include "textureList.h"
#include "minecart.h"
#include "math/bodypart2d.h"
#include "dimension.h"
minecart::minecart(dimension* dimensionIn, cvec2& position) :hollowRidableEntity(dimensionIn, position, entityID::minecart)
{
	mainBodyPart = new bodyPart2D(minecartEditedTextureRect, nullptr, position, minecartRenderSize, minecartRotationCentre);
}

void minecart::render(const gameRenderData& targetData) const
{
	//'snap' on the rails
	mainBodyPart->translate = position;
	mainBodyPart->changed = true;
	mainBodyPart->renderRecursive(targetData.renderTarget, targetData.worldToRenderTargetTransform, *minecartTexture);
}

void minecart::tick()
{
	heal(0.1);
	const vect2<blockID> wheelsOn = getBlockWheelsOn();
	const vect2<veci2> wheelPositions = getwheelPositions();
	for (int i = 0; i < 2; i++)
	{
		if (wheelsOn[i] == blockID::powered_rail)
		{
			const powerLevel level = dimensionIn->getPowerLevel(wheelPositions[i]);
			if (level > 0)
			{
				cfp acceleration = poweredRailsAcceleration * getPowerStrength(level);

				if (speed.x < 0)
				{
					speed.x -= acceleration;
				}
				else if (speed.x > 0)
				{
					speed.x += acceleration;
				}
				else
				{
					for (fsize_t i = 0; i < directionCount1D; i++)
					{
						cvec2 speedNormal = directionVectors2D[i];
						constexpr fp revertDistance = 1.0 / 0x10;
						if (dimensionIn->getHitboxCollisionType(calculateHitBox(position + speedNormal * revertDistance)) != collisionTypeID::willNotCollide)
						{
							speed.x -= speedNormal.x * acceleration;
							break;
						}
					}
				}
			}
		}
	}

	ridableEntity::tick();
}

void minecart::onDeath()
{
	ridableEntity::onDeath();
	//drop as item
	lootTable::dropLoot({ itemStack(itemID::minecart, 1) }, dimensionIn, position, maxFloatingSlotSpeed);
}

vec3 minecart::getGroundFriction() const
{
	const vect2<blockID> wheelsOn = getBlockWheelsOn();

	cbool& hasRail = isRail(wheelsOn.x) || isRail(wheelsOn.y);

	//better calculation
	return hasRail ? vec3(cvec2(), railsFrictionPerBlock * getLengthTouchingGround()) : entity::getGroundFriction();
}

vec2 minecart::getRelativeSeatPosition(csize_t& seatIndex) const
{
	return cvec2(0, minecartHitboxSize.y * 0.5);
}

const vect2<blockID> minecart::getBlockWheelsOn() const
{
	crectangle2 currentHitbox = calculateHitBox();
	cvec2 wheelPositions[2]
	{
		currentHitbox.pos0,
		currentHitbox.pos10()
	};
	vect2<blockID> wheelsOn;

	for (int i = 0; i < 2; i++)
	{
		cveci2 flooredWheelPosition = floorVector(wheelPositions[i]);
		cvec2 substractedWheelPosition = wheelPositions[i] - cvec2(flooredWheelPosition);

		const blockID railsOn = dimensionIn->getBlockID(floorVector(wheelPositions[i] + cvec2(0, -sleepersHeight)));
		if (isRail(railsOn))
		{
			cvecb2 topConnection = dimensionIn->railTopConnection(flooredWheelPosition);

			cfp lerpValue = math::lerp(topConnection[0] ? (fp)1 : (fp)0, topConnection[1] ? (fp)1 : (fp)0, substractedWheelPosition.x);
			if (substractedWheelPosition.y < lerpValue + railsHeight && substractedWheelPosition.y > sleepersHeight)
			{
				wheelsOn[i] = railsOn;
			}
			else
			{
				wheelsOn[i] = blockID::air;
			}
		}
		else
		{
			wheelsOn[i] = railsOn;
		}
	}
	return wheelsOn;
}

const vect2<veci2> minecart::getwheelPositions() const
{
	crectangle2 currentHitbox = calculateHitBox();
	return vect2<veci2>(floorVector(currentHitbox.pos0 + cvec2(0, -sleepersHeight)), floorVector(currentHitbox.pos10() + cvec2(0, -sleepersHeight)));
}

fp minecart::getLengthTouchingGround() const
{
	return minecartWheelSize * 2;
}

fp minecart::getHollowVolume() const
{
	return getVolume() * 0.9;
}

fp minecart::getWeightPerCubicMeter() const
{
	return ironWeightPerCubicMeter;
}