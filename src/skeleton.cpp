#include "skeleton.h"
#include "dimension.h"
#include "humanoid.h"
#include "taskList.h"
#include "arrowAttackAI.h"
#include "wanderAI.h"
#include "hurtByTargetAI.h"
skeleton::skeleton(dimension *dimensionIn, cvec2 &position, const entityID &entityType) : humanoid(dimensionIn, position, entityType)
{
	tasks = new taskList(this, {
								   new hurtByTargetAI(this),
								   new arrowAttackAI(this, entityID::human),
								   new attackOnCollideAI(this, entityID::human),
								   new wanderAI(this),
							   });

	initializeBodyParts(skeletonHeadTextureRect, skeletonBodyTextureRect, skeletonLeftLegTextureRect, skeletonRightLegTextureRect, skeletonLeftArmTextureRect, skeletonRightArmTextureRect, skeletonPixelSize);
	mainBodyPart->hasTransparency = true;
	itemHolding = new itemStack(itemID::bow, 1);
}

void skeleton::serializeValue(nbtSerializer &s)
{
	humanoid::serializeValue(s);
	itemHolding->serialize(s, std::wstring(L"item holding"));
}