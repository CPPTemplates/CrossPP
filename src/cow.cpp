#include "cow.h"
cow::cow(dimension *dimensionIn, cvec2 &position) : mob(dimensionIn, position, entityID::cow), fourLeggedPassiveMob(dimensionIn, position, entityID::cow)
{
	// intialize body parts
	mainBodyPart = new bodyPart2D(cowBodyTextureRect, nullptr, vec2(position.x, position.y + cowLegSize.y), cowBodySize, cowBodyRotationCentre, 0, 90);

	frontRightLeg = new bodyPart2D(cowFrontRightLegTextureRect, mainBodyPart, cowFrontLegOffset, cowLegSize, cowLegRotationCentre);
	frontLeftLeg = new bodyPart2D(cowFrontLeftLegTextureRect, mainBodyPart, cowFrontLegOffset, cowLegSize, cowLegRotationCentre);

	backRightLeg = new bodyPart2D(cowFrontRightLegTextureRect, mainBodyPart, cowBackLegOffset, cowLegSize, cowLegRotationCentre);
	backLeftLeg = new bodyPart2D(cowFrontLeftLegTextureRect, mainBodyPart, cowBackLegOffset, cowLegSize, cowLegRotationCentre);

	head = new bodyPart2D(cowHeadTextureRect, mainBodyPart, vec2(cowBodySize.x - cowBodyRotationCentre.x - cowHeadOffset + cowHeadRotationCentre.x, cowBodySize.y + cowHeadOffset - cowHeadRotationCentre.y), cowHeadSize, cowHeadRotationCentre);
	connectBodyParts();
}