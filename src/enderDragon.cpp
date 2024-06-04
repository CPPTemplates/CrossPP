#include "enderDragon.h"
#include "dimension.h"
#include "taskList.h"
#include "enderDragonPortalFlyingAI.h"
#include "panicAI.h"
#include "enderDragonStrafingAI.h"
#include "enderDragonCirclingAI.h"
#include "dragonBeamBrush.h"
#include "math/swingsynchronizer.h"
#include "math/timemath.h"
#include "idAnalysis.h"
#include "soundCollection.h"
#include "mobDamageSource.h"
#include "end.h"
#include "structure.h"
#include "experienceOrb.h"
#include "folderList.h"
#include "world.h"

//convert seconds to relative arm angle
const swingSynchronizer enderDragonInnerWingSynchronizer = swingSynchronizer(2, -10 * math::degreesToRadians, 40 * math::degreesToRadians);
const swingSynchronizer enderDragonOuterWingSynchronizer = swingSynchronizer(2, -50 * math::degreesToRadians, 40 * math::degreesToRadians, 0.5);

enderDragon::enderDragon(dimension* dimensionIn, cvec2& position) :mob(dimensionIn, position, entityID::ender_dragon)
{
	tasks = new taskList(this, {
		new enderDragonPortalFlyingAI(this) ,
		new panicAI(this) ,
		new enderDragonStrafingAI(this) ,
		new enderDragonCirclingAI(this)
		});

	flying = true;

	collideLevel = collisionTypeID::willNotCollide;

	//fill
	dyingBeams = new vec3[dragonDyingBeamCount];
	for (int i = 0; i < dragonDyingBeamCount; i++)
	{
		dyingBeams[i] = vec3(randFp(math::PI2), randFp(worldRandom, dragonDyingBeamMaxWidth), randFp(worldRandom));
	}

	cvec2 enderDragonHeadSize = cvec2(enderDragonHeadTextureRect.size) * enderDragonPixelSize;

	head = new bodyPart2D(enderDragonHeadTextureRect, nullptr, vec2(0, 0), enderDragonHeadSize, cvec2(0, enderDragonHeadSize.y * 0.5));
	mainBodyPart = head;

	cvec2 enderDragonHeadPlateSize = cvec2(enderDragonLeftHeadPlateTextureRect.size) * enderDragonPixelSize;
	bodyPart2D* rightHeadPlate = new bodyPart2D(enderDragonRightHeadPlateTextureRect, head, cvec2(enderDragonHeadPlateSize.x, head->size.y) - head->rotationCentre, enderDragonHeadPlateSize, cvec2(enderDragonHeadPlateSize.x * 0.5, 0));
	bodyPart2D* leftHeadPlate = new bodyPart2D(enderDragonLeftHeadPlateTextureRect, head, cvec2(enderDragonHeadPlateSize.x, head->size.y) - head->rotationCentre, enderDragonHeadPlateSize, cvec2(enderDragonHeadPlateSize.x * 0.5, 0));

	cvec2 enderDragonLowerJawSize = cvec2(enderDragonLowerJawTextureRect.size) * enderDragonPixelSize;
	cvec2 enderDragonUpperJawSize = cvec2(enderDragonUpperJawTextureRect.size) * enderDragonPixelSize;
	cvec2 enderDragonJawRotationCentre = cvec2(head->size.x, (enderDragonLowerJawSize.y + enderDragonUpperJawSize.y) * 0.5) - head->rotationCentre;

	lowerJaw = new bodyPart2D(enderDragonLowerJawTextureRect, head, enderDragonJawRotationCentre, enderDragonLowerJawSize, cvec2(0, enderDragonLowerJawSize.y));

	bodyPart2D* upperJaw = new bodyPart2D(enderDragonUpperJawTextureRect, head, enderDragonJawRotationCentre, enderDragonUpperJawSize, cvec2());

	cvec2 enderDragonNostrilSize = cvec2(enderDragonNostrilTextureRect.size) * enderDragonPixelSize;
	bodyPart2D* rightNostril = new bodyPart2D(enderDragonNostrilTextureRect, upperJaw, cvec2(upperJaw->size.x - enderDragonNostrilSize.x, upperJaw->size.y) - upperJaw->rotationCentre, enderDragonNostrilSize, cvec2(enderDragonNostrilSize.x * 0.5, 0));
	bodyPart2D* leftNostril = new bodyPart2D(enderDragonNostrilTextureRect, upperJaw, cvec2(upperJaw->size.x - enderDragonNostrilSize.x, upperJaw->size.y) - upperJaw->rotationCentre, enderDragonNostrilSize, cvec2(enderDragonNostrilSize.x * 0.5, 0));

	createSpine(head, neckSpines, enderDragonNeckSpineCount);

	body = new bodyPart2D(enderDragonBodyTextureRect, neckSpines[enderDragonNeckSpineCount - 1], vec2(-enderDragonspineSize.x, 0), enderDragonBodySize, cvec2(enderDragonBodySize.x, enderDragonBodySize.y * 0.5));
	bodyPart2D* bodySpinePlates[enderDragonBodySpinePlateCount];
	for (int i = 0; i < enderDragonBodySpinePlateCount; i++)
	{
		cvec2 enderDragonBodySpinePlateSize = cvec2(enderDragonBodySpineTextureRect.size) * enderDragonPixelSize;
		bodySpinePlates[i] = new bodyPart2D(enderDragonBodySpineTextureRect, body, cvec2((body->size.x / enderDragonBodySpinePlateCount) * (i + 0.5), body->size.y) - body->rotationCentre, enderDragonBodySpinePlateSize, cvec2(enderDragonBodySpinePlateSize.x * 0.5, 0));
	}

	createSpine(body, tailSpines, enderDragonTailSpineCount);

	cvec2 frontLegAttachmentPoint = cvec2(enderDragonBodySize.x - frontAttachmentOffset, frontAttachmentOffset) - body->rotationCentre;

	bodyPart2D* frontRightLeg = createDragonLeg(frontLegAttachmentPoint, enderDragonFrontRightUpperLegTextureRect, enderDragonFrontUpperLegAngle, enderDragonFrontRightLowerLegTextureRect, enderDragonFrontLowerLegAngle, enderDragonFrontRightFootTextureRect, enderDragonFrontFootAngle);
	bodyPart2D* frontLeftLeg = createDragonLeg(frontLegAttachmentPoint, enderDragonFrontLeftUpperLegTextureRect, enderDragonFrontUpperLegAngle, enderDragonFrontLeftLowerLegTextureRect, enderDragonFrontLowerLegAngle, enderDragonFrontLeftFootTextureRect, enderDragonFrontFootAngle);

	cvec2 wingAttachmentPoint = body->size + cvec2(-frontAttachmentOffset) - body->rotationCentre;

	rightInnerWingTop = new bodyPart2D(enderDragonInnerWingTopTextureRect, body, wingAttachmentPoint, enderDragonInnerWingTopSize, cvec2(enderDragonInnerWingTopSize.x, 0), 0, 90, true);
	rightInnerWingTop->flipX = true;
	leftInnerWingTop = new bodyPart2D(enderDragonInnerWingTopTextureRect, body, wingAttachmentPoint, enderDragonInnerWingTopSize, cvec2(enderDragonInnerWingTopSize.x, 0), 0, 90, true);
	leftInnerWingTop->flipX = true;
	rightOuterWingTop = new bodyPart2D(enderDragonOuterWingTopTextureRect, rightInnerWingTop, cvec2(), enderDragonOuterWingTopSize, cvec2(enderDragonOuterWingTopSize.x, 0), 0, 90, true);
	leftOuterWingTop = new bodyPart2D(enderDragonOuterWingTopTextureRect, leftInnerWingTop, cvec2(), enderDragonOuterWingTopSize, cvec2(enderDragonOuterWingTopSize.x, 0), 0, 90, true);

	rightOuterWingSide = new bodyPart2D(enderDragonOuterWingSideTextureRect, rightOuterWingTop, cvec2(), enderDragonOuterWingSideSize, cvec2(enderDragonOuterWingSideSize.x, 0), 0, 90, true);
	leftOuterWingSide = new bodyPart2D(enderDragonOuterWingSideTextureRect, leftOuterWingTop, cvec2(), enderDragonOuterWingSideSize, cvec2(enderDragonOuterWingSideSize.x, 0), 0, 90, true);

	cvec2 backLegAttachmentPoint = cvec2(enderDragonBodySize.y * 0.5 - enderDragonBodySize.x, 0);

	bodyPart2D* backRightLeg = createDragonLeg(backLegAttachmentPoint, enderDragonBackRightUpperLegTextureRect, enderDragonBackUpperLegAngle, enderDragonBackRightLowerLegTextureRect, enderDragonBackLowerLegAngle, enderDragonBackRightFootTextureRect, enderDragonBackFootAngle);
	bodyPart2D* backLeftLeg = createDragonLeg(backLegAttachmentPoint, enderDragonBackLeftUpperLegTextureRect, enderDragonBackUpperLegAngle, enderDragonBackLeftLowerLegTextureRect, enderDragonBackLowerLegAngle, enderDragonBackLeftFootTextureRect, enderDragonBackFootAngle);

	rightInnerWingTop->children.push_back(rightOuterWingTop);
	leftInnerWingTop->children.push_back(leftOuterWingTop);
	leftInnerWingTop->drawOrderIndex = leftInnerWingTop->children.size();

	rightOuterWingTop->children.push_back(rightOuterWingSide);
	leftOuterWingTop->children.push_back(leftOuterWingSide);
	leftOuterWingTop->drawOrderIndex = leftOuterWingTop->children.size();

	//left parts at the back, right parts at front
	body->children.push_back(leftInnerWingTop);
	body->children.push_back(frontLeftLeg);
	body->children.push_back(backLeftLeg);
	body->children.push_back(tailSpines[0]);
	body->drawOrderIndex = body->children.size();

	for (int i = 0; i < enderDragonBodySpinePlateCount; i++)
	{
		body->children.push_back(bodySpinePlates[i]);
	}

	body->children.push_back(frontRightLeg);
	body->children.push_back(backRightLeg);
	body->children.push_back(rightInnerWingTop);

	neckSpines[enderDragonNeckSpineCount - 1]->children.push_back(body);

	upperJaw->children.push_back(leftNostril);
	upperJaw->drawOrderIndex = upperJaw->children.size();
	upperJaw->children.push_back(rightNostril);

	head->children.push_back(leftHeadPlate);
	head->children.push_back(lowerJaw);
	head->children.push_back(upperJaw);
	head->drawOrderIndex = head->children.size();
	head->children.push_back(neckSpines[0]);
	head->children.push_back(rightHeadPlate);
}

bodyPart2D* enderDragon::createDragonLeg(cvec2& attachmentPoint, crectangle2& upperLegTextureRect, cfp& upperLegAngle, crectangle2& lowerLegTextureRect, cfp& lowerLegAngle, crectangle2& footTextureRect, cfp& footAngle)
{
	cvec2 upperLegSize = cvec2(upperLegTextureRect.size) * enderDragonPixelSize;
	bodyPart2D* upperLeg = new bodyPart2D(upperLegTextureRect, body, attachmentPoint, upperLegSize, cvec2(upperLegSize.x * 0.5, upperLegSize.y - upperLegSize.x * 0.5), upperLegAngle);

	//attachment: substract half the width two times and negate

	cvec2 lowerLegSize = cvec2(lowerLegTextureRect.size) * enderDragonPixelSize;
	bodyPart2D* lowerLeg = new bodyPart2D(lowerLegTextureRect, upperLeg, cvec2(0, -upperLegSize.y + upperLegSize.x), lowerLegSize, cvec2(lowerLegSize.x * 0.5, lowerLegSize.y - lowerLegSize.x * 0.5), lowerLegAngle);

	cvec2 footSize = cvec2(footTextureRect.size) * enderDragonPixelSize;
	bodyPart2D* foot = new bodyPart2D(footTextureRect, lowerLeg, -lowerLeg->rotationCentre, footSize, cvec2(), footAngle);

	upperLeg->children.push_back(lowerLeg);
	lowerLeg->children.push_back(foot);

	return upperLeg;
}

void enderDragon::createSpine(bodyPart2D* attachTo, bodyPart2D** spinePtr, cint spineCount)
{
	for (int i = 0; i < spineCount; i++)
	{
		bodyPart2D* currentVertebra = new bodyPart2D(enderDragonSpineTextureRect, attachTo, cvec2(0, attachTo->size.y * 0.5) - attachTo->rotationCentre, enderDragonspineSize, cvec2(enderDragonspineSize.x, enderDragonspineSize.y * 0.5));

		cvec2 enderDragonSpinePlateSize = cvec2(enderDragonSpinePlateTextureRect.size) * enderDragonPixelSize;
		bodyPart2D* spinePlate = new bodyPart2D(enderDragonSpinePlateTextureRect, currentVertebra, cvec2(currentVertebra->size.x * 0.5, currentVertebra->size.y) - currentVertebra->rotationCentre, enderDragonSpinePlateSize, cvec2(enderDragonSpinePlateSize.x * 0.5, 0));
		currentVertebra->children.push_back(spinePlate);

		if (i > 0)
		{
			attachTo->children.push_back(currentVertebra);
		}
		spinePtr[i] = currentVertebra;
		attachTo = currentVertebra;
	}
}

void enderDragon::render(const gameRenderData& targetData) const
{
	//render death beams
	if (deathAnimationTicks)
	{
		cfp deathAnimationPart = deathAnimationTicks / (fp)enderDragonDeathAnimationDuration;
		for (int i = 0; i < dragonDyingBeamCount; i++)
		{
			cfp beamLengthMultiplier = deathAnimationPart * dyingBeams[i].z;
			cfp whiteDistance = dragonDyingBeamPurpleDistance * beamLengthMultiplier;
			cfp transparencyDistance = dragonDyingBeamTransparencyDistance * beamLengthMultiplier;

			cvec2 p0 = position + vec2::getrotatedvector(dyingBeams[i].x) * transparencyDistance;
			cvec2 p1 = position + vec2::getrotatedvector(dyingBeams[i].x + dyingBeams[i].y) * transparencyDistance;
			fillPolygon(targetData.renderTarget, {
				targetData.worldToRenderTargetTransform.multPointMatrix(position),
				targetData.worldToRenderTargetTransform.multPointMatrix(p0),
				targetData.worldToRenderTargetTransform.multPointMatrix(p1)
				},
				dragonBeamBrush<texture>(&targetData.renderTarget, targetData.worldToRenderTargetTransform.multPointMatrix(position),
					targetData.worldToRenderTargetTransform.multSizeMatrix(cvec1(whiteDistance)).x,
					targetData.worldToRenderTargetTransform.multSizeMatrix(cvec1(transparencyDistance)).x));
		}
	}
	//render healing beam
	else if (uuidHealingFrom)
	{
		entity* endCrystal = dimensionIn->findUUID(position, endCrystalHealRange, uuidHealingFrom);
		if (endCrystal)
		{
			//should be: position - endcrystal->position?
			cvec2& difference = position - endCrystal->position;
			cfp& beamRotation = difference.normalized().getRotation();//0 = up
			//cvec2 leftRelativePos = endCrystal->position + vec2::getrotatedvector(beamRotation - (90 * math::degreesToRadians));
			cfp& beamLength = difference.length();

			mat3x3 beamTransform = mat3x3::combine({
				mat3x3::fromRectToRect(crectangle2(endCrystalBeamTextureRect), crectangle2(endCrystalBeamWidth * -0.5, 0, endCrystalBeamWidth, beamLength)),
				mat3x3::rotate(beamRotation),
				mat3x3::translate(endCrystal->position),
				targetData.worldToRenderTargetTransform
				});
			fillTransparentRectangle(crectangle2(endCrystalBeamTextureRect), beamTransform, *endCrystalBeamTexture, targetData.renderTarget);
		}
	}

	updateBodyParts();

	renderBodyPart(mainBodyPart, targetData.worldToRenderTargetTransform, targetData);
}

void enderDragon::tick()
{
	mob::tick();
	if (enderDragonInnerWingSynchronizer.maximumBetween(currentWorld->ticksSinceStart * secondsPerTick, (currentWorld->ticksSinceStart + 1) * secondsPerTick))
	{
		enderDragonWingsSound->playRandomSound(dimensionIn, position);
	}

	//not: if(health). health can be below 0
	if (health > 0)
	{
		//for all hitboxes, check if they hit some blocks. if yes, destroy them
		std::vector<entity*> collidingEntities = dimensionIn->getCollidingEntities(calculateHitBox());
		for (entity* e : collidingEntities)
		{
			if (isMob(e->entityType) && e != this)
			{
				//cvec2 knockBack = speed + cvec2(0, randFp(currentRandom, enderDragonMaxAdditionalUpwardsKnockbackForce));
				e->addDamageSource(enderDragonDamage, std::make_shared<mobDamageSource>(identifier));
				speed = e->handleCollision(speed, getWeight());
			}
		}
		//find crystals to heal from
		std::vector<entity*> nearEntities = dimensionIn->findNearEntities(position, endCrystalHealRange);
		entity* nearestEndCrystal = nullptr;
		fp endCrystalDistanceSquared = INFINITY;
		for (entity* e : nearEntities)
		{
			if (e->entityType == entityID::end_crystal)
			{
				cfp distanceSquared = (e->position - position).lengthSquared();
				if (distanceSquared < endCrystalDistanceSquared)
				{
					endCrystalDistanceSquared = distanceSquared;
					nearestEndCrystal = e;
				}
			}
		}
		if (nearestEndCrystal)
		{
			heal(2 * secondsPerTick);
			uuidHealingFrom = nearestEndCrystal->identifier;
		}
		else
		{
			uuidHealingFrom = uuid();
		}
	}
}

bool enderDragon::addDamageSource(cfp& damage, std::shared_ptr<damageSource> source)
{
	if ((source.get()->type == blastDamage) || (source.get()->type == projectileDamage) || (source.get()->type == mobDamage))
	{
		return mob::addDamageSource(damage, source);
	}
	else
	{
		return false;
	}
}

void enderDragon::onDeath()
{
	if (deathAnimationTicks >= enderDragonDeathAnimationDuration)
	{
		mob::onDeath();
		if (dimensionIn->identifier == dimensionID::end)
		{
			end* currentEnd = (end*)dimensionIn;
			if (currentEnd->dragonAlive)
			{
				currentEnd->dragonAlive = false;

				//add end portal and egg
				const structure* endExitPortalStructure = getStructureByPath(L"the_end/exit_portal");
				cveci2 dragonEggPosition = currentEnd->exitPortalLevel + cveci2(0, (int)endExitPortalStructure->blockIDArray.size.y);
				dimensionIn->setBlockID(dragonEggPosition, blockID::dragon_egg);

				cveci2& exitPortalLocation = cveci2(0, currentEnd->exitPortalLevel);

				//left
				dimensionIn->setBlockRange(exitPortalLocation + cveci2(-2, 1), exitPortalLocation + cveci2(-1, 1), blockID::end_portal);
				//right
				dimensionIn->setBlockRange(exitPortalLocation + cveci2(1, 1), exitPortalLocation + cveci2(2, 1), blockID::end_portal);
			}
		}
		//experience explosion
		for (int i = 0; i < 11; i++)
		{
			experienceOrb* orb = (experienceOrb*)summonEntity(entityID::experience_orb, dimensionIn, position);
			orb->value = i ? 960 : 2400;
			//drop experience in all directions with a speed of 10m/s
			orb->speed = vec2::getrotatedvector(randFp(math::PI2)) * randFp((fp)10);
		}
	}
	currentPhase = dragonPhase::flyingToPortalToDie;
}

enderDragon::~enderDragon()
{

	delete[] dyingBeams;
}

void enderDragon::updateBodyParts() const
{
	//update angles

	vec2 relativeHeadPosition = cvec2(enderDragonHitboxSize.x * 0.5, 0) + cvec2(-(lowerJaw->size.x), head->size.y * -0.5) - head->rotationCentre;
	if (mainBodyPart->flipX)
	{
		relativeHeadPosition.x = -relativeHeadPosition.x;
	}
	else
	{
		relativeHeadPosition.x -= head->size.x;
	}
	head->translate = position + relativeHeadPosition;

	head->changed = true;

	//0 = start, 1 = end

	cfp innerWingAngle = enderDragonInnerWingSynchronizer.getSwingAngle(currentWorld->ticksSinceStart * secondsPerTick);
	cfp outerWingAngle = enderDragonOuterWingSynchronizer.getSwingAngle(currentWorld->ticksSinceStart * secondsPerTick);

	cfp innerWingTopSize = sin(innerWingAngle) * (enderDragonInnerWingTopSize.y);
	fp outerWingTopSize = sin(outerWingAngle) * (enderDragonOuterWingTopSize.y);

	fp innerWingSize;
	fp innerWingRotationCentre;

	fp outerWingOffset;
	fp outerWingSize;
	fp outerWingRotationCentre;

	fp outerWingSideOffset;

	if (innerWingTopSize < 0)
	{
		innerWingSize = -innerWingTopSize;
		innerWingRotationCentre = innerWingSize;
		outerWingTopSize = -outerWingTopSize;
		outerWingOffset = 0;
	}
	else
	{
		innerWingSize = innerWingTopSize;
		innerWingRotationCentre = 0;
		outerWingOffset = innerWingSize;
	}

	//all beneath can be flipped

	if (outerWingTopSize < 0)
	{
		outerWingSize = -outerWingTopSize;
		outerWingRotationCentre = outerWingSize;
		outerWingSideOffset = enderDragonOuterWingSideSize.y;
	}
	else
	{
		outerWingRotationCentre = 0;
		outerWingSize = outerWingTopSize;
		outerWingSideOffset = 0;
	}

	leftInnerWingTop->size.y = innerWingSize;
	leftInnerWingTop->rotationCentre.y = innerWingRotationCentre;
	leftInnerWingTop->flipY = innerWingTopSize < 0;

	rightInnerWingTop->size.y = innerWingSize;
	rightInnerWingTop->rotationCentre.y = innerWingRotationCentre;
	rightInnerWingTop->flipY = innerWingTopSize < 0;

	leftOuterWingTop->size.y = outerWingSize;
	leftOuterWingTop->rotationCentre.y = outerWingRotationCentre;
	leftOuterWingTop->translate.y = outerWingOffset;
	leftOuterWingTop->flipY = outerWingTopSize < 0;

	rightOuterWingTop->size.y = outerWingSize;
	rightOuterWingTop->rotationCentre.y = outerWingRotationCentre;
	rightOuterWingTop->translate.y = outerWingOffset;
	rightOuterWingTop->flipY = outerWingTopSize < 0;

	leftOuterWingSide->rotationCentre.y = outerWingSideOffset;
	leftOuterWingSide->translate.y = outerWingSideOffset;

	rightOuterWingSide->rotationCentre.y = outerWingSideOffset;
	rightOuterWingSide->translate.y = outerWingSideOffset;

	leftInnerWingTop->changed = true;
	rightInnerWingTop->changed = true;

	leftOuterWingTop->changed = true;
	rightOuterWingTop->changed = true;

	leftOuterWingSide->changed = true;
	rightOuterWingSide->changed = true;
}