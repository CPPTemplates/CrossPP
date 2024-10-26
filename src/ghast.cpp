#include "math/swingsynchronizer.h"
#include "dimension.h"
#include "ghast.h"
#include "taskList.h"
#include "ghastFireBallAI.h"
#include "wanderAI.h"
ghast::ghast(dimension* dimensionIn, cvec2& position) :mob(dimensionIn, position, entityID::ghast)
{
	tasks = new taskList(this, {
	new ghastFireBallAI(this),
	new wanderAI(this),
		});

	flying = true;

	collideLevel = collisionTypeID::willCollide;

	head = new bodyPart2D(ghastHeadTextureRect, nullptr, position, ghastHeadSize, ghastHeadRotationCentre);
	mainBodyPart = head;

	for (int i = 0; i < ghastLegCount; i++)
	{
		cfp legLength = randFp(currentRandom, ghastMaxLegSize.y * 0.5, ghastMaxLegSize.y);
		legs[i] = new bodyPart2D(ghastLegTextureRect, head, cvec2(randFp(currentRandom, ghastMaxLegSize.x * 0.5, ghastHeadSize.x - ghastMaxLegSize.x * 0.5), 0) - ghastHeadRotationCentre, cvec2(ghastMaxLegSize.x, legLength), cvec2(ghastMaxLegSize.x * 0.5, legLength));
		head->children.push_back(legs[i]);
		legSynchronizers[i] = swingSynchronizer(randFp(currentRandom, (fp)0x4, (fp)0x8), 20 * math::degreesToRadians, 40 * math::degreesToRadians);
	}
	//all legs wil always be behind the ghast, even though they are randomly distributed.
	head->drawOrderIndex = ghastLegCount;
}

void ghast::updateBodyParts() const
{
	mainBodyPart->translate = position;
	mainBodyPart->changed = true;
	for (int i = 0; i < ghastLegCount; i++)
	{
		legs[i]->angle = legSynchronizers[i].getSwingAngle(totalLegDistance);
		legs[i]->changed = true;
	}
}