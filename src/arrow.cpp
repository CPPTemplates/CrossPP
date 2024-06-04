#include "throwableData.h"
#include "arrow.h"
#include "math/bodypart2d.h"
#include "soundList.h"
#include "dimension.h"
arrow::arrow(dimension* dimensionIn, cvec2& position) : throwable(dimensionIn, position, entityID::arrow)
{
	mainBodyPart = new bodyPart2D(arrowTextureRect, nullptr, position, arrowSize, arrowSize * 0.5);
	mainBodyPart->hasTransparency = true;
}
void arrow::render(const gameRenderData& targetData) const
{
	mainBodyPart->translate = position;
	if (speed.x != 0 || speed.y != 0)
	{
		cvec2 normalizedSpeed = speed.normalized();
		cfp speedRotation = normalizedSpeed.getRotation();
		mainBodyPart->angle = speedRotation - math::PI2 * 0.25;
	}
	else
	{
		mainBodyPart->angle = impactAngle;
	}

	mainBodyPart->changed = true;
	mainBodyPart->renderRecursive(targetData.renderTarget, targetData.worldToRenderTargetTransform, *((throwableData*)entityDataList[(int)entityType])->tex);
}
void arrow::onCollision(const std::vector<entity*>& collidingEntities)
{
	cfp currentSpeedPerSecond = speed.length();
	cfp arrowSpeedPercentage = currentSpeedPerSecond / ((throwableData*)entityDataList[(int)entityType])->throwPower;
	fp arrowDamage = arrowImpactDamage * arrowSpeedPercentage;
	if (arrowSpeedPercentage > 0.8)
	{
		if (randChance(currentRandom, 4))
		{
			//critical hit
			arrowDamage++;
		}
	}

	addImpactDamage(collidingEntities, arrowDamage);
	bowHitSound->playRandomSound(dimensionIn, position);
	despawn = true;
}