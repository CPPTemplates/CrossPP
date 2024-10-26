#include "pig.h"
pig::pig(dimension* dimensionIn, cvec2& position) : mob(dimensionIn, position, entityID::pig), fourLeggedPassiveMob(dimensionIn, position, entityID::pig)
{

	//intialize body parts
	mainBodyPart = new bodyPart2D(pigBodyTextureRect, nullptr, vec2(position.x, position.y + pigLegSize.y), pigBodySize, pigBodyRotationCentre, 0, 90);

	frontRightLeg = new bodyPart2D(pigFrontRightLegTextureRect, mainBodyPart, pigFrontLegOffset, pigLegSize, pigLegRotationCentre);
	frontLeftLeg = new bodyPart2D(pigFrontLeftLegTextureRect, mainBodyPart, pigFrontLegOffset, pigLegSize, pigLegRotationCentre);

	backRightLeg = new bodyPart2D(pigFrontRightLegTextureRect, mainBodyPart, pigBackLegOffset, pigLegSize, pigLegRotationCentre);
	backLeftLeg = new bodyPart2D(pigFrontLeftLegTextureRect, mainBodyPart, pigBackLegOffset, pigLegSize, pigLegRotationCentre);

	head = new bodyPart2D(pigHeadTextureRect, mainBodyPart, vec2(pigBodySize.x - pigBodyRotationCentre.x - pigHeadOffset + pigHeadRotationCentre.x, pigBodySize.y + pigHeadOffset - pigHeadRotationCentre.y), pigHeadSize, pigHeadRotationCentre);
	connectBodyParts();
}