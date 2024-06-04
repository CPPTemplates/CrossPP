#include "stareDownAI.h"
#include "dimension.h"
#include "mob.h"
#include "enderman.h"
#include "soundList.h"
bool stareDownAI::shouldExecute()
{
	if (hurtByTargetAI::shouldExecute()) {
		return true;
	}
	//check if any entity, except for the player, is staring into it
	auto l = connectedEntity->dimensionIn->findNearEntities(connectedEntity->position, proximity + mobSizeMargin);
	for (const entity* e : l) {
		if (isHumanoid(e->entityType) && e->entityType != entityID::enderman) {

			const mob* m = (mob*)e;
			m->updateBodyParts();
			cvec2& mobHeadPos = m->getHeadPosition();
			if (m->lookingAt == mobHeadPos) continue;//to prevent normalisation issues. the mob is probably stuck inside a block or something
			cvec2& mobLookingAt = m->lookingAt - mobHeadPos;
			cvec2& directionNormal = mobLookingAt.normalized();
			cvec2& endermanHeadPos = ((mob*)connectedEntity)->getHeadPosition();
			fp tMin, tMax;//dummy
			//check if a ray cast from the eye of the mob collides with the head of the enderman
			if (collidedistance2d(mobHeadPos, mobHeadPos + directionNormal * proximity,
				crectangle2(endermanHeadPos, cvec2()).expanded(endermanHeadTextureRect.size.x * endermanPixelSize * 0.5), tMin, tMax)
				)
			{
				if (targetNear(m)) {
					target = m->identifier;
					ticksAngry = maxTicksAngry;
					endermanStareSound->playRandomSound(connectedEntity->dimensionIn, endermanHeadPos);
					return true;
				}
			}
		}
	}
	return false;
}
