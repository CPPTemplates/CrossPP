#include "fourLeggedMob.h"
#include "mobData.h"
fourLeggedMob::fourLeggedMob(dimension* dimensionIn, cvec2& position, const entityID& entityType) :mob(dimensionIn, position, entityType)
{
}
void fourLeggedMob::updateBodyParts() const
{
	mainBodyPart->translate = vec2(position.x, position.y + frontRightLeg->size.y);
	//update leg positions
	cfp rightLegangle = ((mobData*)entityDataList[(int)entityType])->legSwingSynchronizer.getSwingAngle(totalLegDistance);
	frontRightLeg->angle = rightLegangle;
	backRightLeg->angle = rightLegangle;
	frontLeftLeg->angle = -rightLegangle;
	backLeftLeg->angle = -rightLegangle;

	frontRightLeg->changed = true;
	backRightLeg->changed = true;
	frontLeftLeg->changed = true;
	backLeftLeg->changed = true;

	mainBodyPart->changed = true;
	updateHeadAngle();
}

void fourLeggedMob::connectBodyParts()
{
	//add bodyparts
	//ordered by draw order
	//left parts at the back, right parts at front
	mainBodyPart->children.push_back(backLeftLeg);
	mainBodyPart->children.push_back(frontLeftLeg);
	mainBodyPart->drawOrderIndex = 2;
	mainBodyPart->children.push_back(head);
	mainBodyPart->children.push_back(backRightLeg);
	mainBodyPart->children.push_back(frontRightLeg);
}