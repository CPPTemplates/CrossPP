#include "wolf.h"
#include "taskList.h"
#include "attackOnCollideAI.h"
#include "wanderAI.h"
#include "hurtByTargetAI.h"
wolf::wolf(dimension* dimensionIn, cvec2& position) :mob(dimensionIn, position, entityID::wolf), fourLeggedMob(dimensionIn, position, entityID::wolf)
{
	tasks = new taskList(this, {
		new hurtByTargetAI(this),
		new attackOnCollideAI(this, entityID::skeleton),
		new wanderAI(this)
	});


	mainBodyPart = new bodyPart2D(wolfBodyTextureRect, nullptr, cvec2(), wolfBodySize, cvec2(wolfBodySize.x * 0.5, 0), 0, 90);

	bodyPart2D* tail = new bodyPart2D(wolfTailTextureRect, mainBodyPart, cvec2(mainBodyPart->size.x * -0.5, mainBodyPart->size.y * 0.5), wolfTailSize, cvec2(wolfTailSize.x, wolfTailSize.y * 0.5), 0, 90);

	mainBodyPart->children.push_back(tail);

	backRightLeg = new bodyPart2D(wolfRightLegTextureRect, mainBodyPart, cvec2(wolfBackLegOffset, 0), wolfLegSize, cvec2(wolfLegSize.x * 0.5, wolfLegSize.y));
	backLeftLeg = new bodyPart2D(wolfLeftLegTextureRect, mainBodyPart, cvec2(wolfBackLegOffset, 0), wolfLegSize, cvec2(wolfLegSize.x * 0.5, wolfLegSize.y));

	frontRightLeg = new bodyPart2D(wolfRightLegTextureRect, mainBodyPart, cvec2(wolfFrontLegOffset, 0), wolfLegSize, cvec2(wolfLegSize.x * 0.5, wolfLegSize.y));
	frontLeftLeg = new bodyPart2D(wolfLeftLegTextureRect, mainBodyPart, cvec2(wolfFrontLegOffset, 0), wolfLegSize, cvec2(wolfLegSize.x * 0.5, wolfLegSize.y));

	mainBodyPart->children.push_back(backRightLeg);
	mainBodyPart->children.push_back(backLeftLeg);
	mainBodyPart->children.push_back(frontRightLeg);
	mainBodyPart->children.push_back(frontLeftLeg);

	bodyPart2D* neck = new bodyPart2D(wolfNeckTextureRect, mainBodyPart, cvec2(wolfBodySize.x * 0.5, wolfBodySize.y * 0.5), wolfNeckSize, cvec2(0, wolfNeckSize.y * 0.5), 0, 90);



	head = new bodyPart2D(wolfHeadTextureRect, neck, cvec2(wolfNeckSize.x, 0), wolfHeadSize, cvec2(0, wolfHeadSize.y * 0.5));

	bodyPart2D* rightEar = new bodyPart2D(wolfRightEarTextureRect, head, cvec2(wolfEarSize.x * 0.5, wolfHeadSize.y * 0.5), wolfEarSize, cvec2(wolfEarSize.x * 0.5, 0));
	bodyPart2D* leftEar = new bodyPart2D(wolfLeftEarTextureRect, head, cvec2(wolfEarSize.x * 0.5, wolfHeadSize.y * 0.5), wolfEarSize, cvec2(wolfEarSize.x * 0.5, 0));

	head->children.push_back(rightEar);
	head->children.push_back(leftEar);

	bodyPart2D* muzzle = new bodyPart2D(wolfNeckTextureRect, head, cvec2(wolfHeadSize.x, (wolfMuzzleSize.y - wolfHeadSize.y) * 0.5), wolfMuzzleSize, cvec2(0, wolfMuzzleSize.y * 0.5));

	head->children.push_back(muzzle);

	neck->children.push_back(head);

	head->drawOrderIndex = 3;

	mainBodyPart->children.push_back(neck);

	//draw the neck last
	mainBodyPart->drawOrderIndex = mainBodyPart->children.size() - 1;
}