#pragma once
#include "math/rectangle/rectangletn.h"
#include "mob.h"
#include "dragonPhase.h"
#include "soundCollection.h"
#include "folderList.h"

//ender dragon
constexpr rectangle2 enderDragonBodyTextureRect = crectangle2(0x58, 0xA8, 0x40, 0x18);
constexpr rectangle2 enderDragonBodySpineTextureRect = crectangle2(234, 185, 12, 6);

constexpr rectangle2 enderDragonHeadTextureRect = crectangle2(0x90, 0xC2, 0x10, 0x10);
constexpr rectangle2 enderDragonUpperJawTextureRect = crectangle2(204, 191, 0x10, 5);
constexpr rectangle2 enderDragonLowerJawTextureRect = crectangle2(204, 171, 0x10, 4);
constexpr rectangle2 enderDragonNostrilTextureRect = crectangle2(0x76, 0xFA, 4, 2);


constexpr rectangle2 enderDragonRightHeadPlateTextureRect = crectangle2(0, 0x0F6, 6, 4);
constexpr rectangle2 enderDragonLeftHeadPlateTextureRect = crectangle2(0x08, 0x0F6, 6, 4);
constexpr rectangle2 enderDragonSpinePlateTextureRect = crectangle2(0x38, 0x0F6, 6, 4);

constexpr rectangle2 enderDragonFrontRightUpperLegTextureRect = crectangle2(112, 120, 8, 24);
constexpr rectangle2 enderDragonFrontLeftUpperLegTextureRect = crectangle2(80, 120, 8, 24);
constexpr rectangle2 enderDragonFrontRightLowerLegTextureRect = crectangle2(226, 88, 6, 24);
constexpr rectangle2 enderDragonFrontLeftLowerLegTextureRect = crectangle2(238, 88, 6, 24);
constexpr rectangle2 enderDragonFrontRightFootTextureRect = crectangle2(144, 132, 16, 4);
constexpr rectangle2 enderDragonFrontLeftFootTextureRect = crectangle2(168, 132, 16, 4);

constexpr rectangle2 enderDragonBackRightUpperLegTextureRect = crectangle2(0, 0xD0, 0x10, 0x20);
constexpr rectangle2 enderDragonBackLeftUpperLegTextureRect = crectangle2(0x20, 0xD0, 0x10, 0x20);
constexpr rectangle2 enderDragonBackRightLowerLegTextureRect = crectangle2(196, 212, 12, 0x20);
constexpr rectangle2 enderDragonBackLeftLowerLegTextureRect = crectangle2(220, 212, 12, 0x20);
constexpr rectangle2 enderDragonBackRightFootTextureRect = crectangle2(112, 226, 24, 6);
constexpr rectangle2 enderDragonBackLeftFootTextureRect = crectangle2(154, 226, 24, 6);

constexpr rectangle2 enderDragonOuterRightWingArmTextureRect = crectangle2(112, 112, 4, 4);
constexpr rectangle2 enderDragonOuterLeftWingArmTextureRect = crectangle2(172, 112, 4, 4);
constexpr rectangle2 enderDragonInnerRightWingArmTextureRect = crectangle2(112, 152, 8, 8);
constexpr rectangle2 enderDragonInnerLeftWingArmTextureRect = crectangle2(176, 152, 8, 8);

constexpr rectangle2 enderDragonSpineTextureRect = crectangle2(192, 132, 10, 10);

constexpr int wingSidePixelHeight = 1;

constexpr rectangle2 enderDragonInnerWingTopTextureRect = crectangle2(0, 112, 56 - wingSidePixelHeight / 2, 56);
constexpr rectangle2 enderDragonOuterWingTopTextureRect = crectangle2(0, 56, 56 - wingSidePixelHeight / 2, 56);

constexpr rectangle2 enderDragonInnerWingSideTextureRect = crectangle2(enderDragonInnerWingTopTextureRect.size.getX() / 2, 112, wingSidePixelHeight / 2, 56);
constexpr rectangle2 enderDragonOuterWingSideTextureRect = crectangle2(enderDragonOuterWingTopTextureRect.size.getX() / 2, 56, wingSidePixelHeight / 2, 56);

constexpr int enderDragonNeckSpineCount = 5;
constexpr int enderDragonBodySpinePlateCount = 3;
constexpr int enderDragonTailSpineCount = 0xc;

constexpr fp enderDragonFrontUpperLegAngle = 70 * math::degreesToRadians;
constexpr fp enderDragonFrontLowerLegAngle = -30 * math::degreesToRadians;
constexpr fp enderDragonFrontFootAngle = 30 * math::degreesToRadians;

constexpr fp enderDragonBackUpperLegAngle = 45 * math::degreesToRadians;
constexpr fp enderDragonBackLowerLegAngle = 45 * math::degreesToRadians;
constexpr fp enderDragonBackFootAngle = 45 * math::degreesToRadians;

constexpr vec2 enderDragonHitboxSize = vec2(0x10, 0x8);

constexpr rectangle2 enderDragonHitbox = crectangle2(-enderDragonHitboxSize.getX() * 0.5, -enderDragonHitboxSize.getY() * 0.5, enderDragonHitboxSize.getX(), enderDragonHitboxSize.getY());

constexpr fp enderDragonPixelSize = enderDragonHitboxSize.getX() / (enderDragonSpineTextureRect.size.getX() * (enderDragonNeckSpineCount + enderDragonTailSpineCount) + enderDragonBodyTextureRect.size.getX() + enderDragonHeadTextureRect.size.getX() + enderDragonLowerJawTextureRect.size.getX());

cvec2 enderDragonspineSize = cvec2(enderDragonSpineTextureRect.size) * enderDragonPixelSize;

constexpr fp wingSideHeight = wingSidePixelHeight * enderDragonPixelSize;

cvec2 enderDragonInnerWingSideSize = cvec2(enderDragonInnerWingSideTextureRect.size.x, wingSidePixelHeight) * enderDragonPixelSize;
cvec2 enderDragonOuterWingSideSize = cvec2(enderDragonOuterWingSideTextureRect.size.x, wingSidePixelHeight) * enderDragonPixelSize;

cvec2 enderDragonInnerWingTopSize = cvec2(enderDragonInnerWingTopTextureRect.size) * enderDragonPixelSize;
cvec2 enderDragonOuterWingTopSize = cvec2(enderDragonOuterWingTopTextureRect.size) * enderDragonPixelSize;

cvec2 enderDragonBodySize = cvec2(enderDragonBodyTextureRect.size) * enderDragonPixelSize;
cfp frontAttachmentOffset = enderDragonBodySize.y / 3;

constexpr fp dragonDyingBeamTransparencyDistance = 0x20;
constexpr fp dragonDyingBeamPurpleDistance = dragonDyingBeamTransparencyDistance / 0x4;
constexpr int dragonDyingBeamCount = 0x20;
constexpr fp dragonDyingBeamMaxWidth = math::PI2 / dragonDyingBeamCount;
constexpr veci2 enderDragonRelativeDyingLocation = cveci2(0, 5);//not that high up, so he can be fully seen
constexpr fp enderDragonDyingAnimationDistance = 0x2;
constexpr fp maximalEnderDragonDyingAnimationDistance = 0x80;
constexpr fp enderDragonDyingAscendSpeed = 2;//ascend 2 blocks per second
constexpr int enderDragonDeathAnimationDuration = 200;
constexpr fp endCrystalHealRange = 0x20;
constexpr rectangle2 endCrystalBeamTextureRect = crectangle2(0, 0, 0x10, 0x100);
constexpr fp endCrystalBeamPixelSize = endCrystalHealRange / (fp)endCrystalBeamTextureRect.size.getY();
constexpr fp endCrystalBeamWidth = endCrystalBeamTextureRect.size.getX() * endCrystalBeamPixelSize;
extern resolutionTexture* endCrystalBeamTexture;
extern std::shared_ptr<soundCollection> enderDragonWingsSound;

//https://sueburke.site/2020/02/06/how-much-does-a-dragon-weigh/#:~:text=A%20dragon%2030%20feet%20long,wingspan%20would%20weigh%20100%20pounds.
constexpr fp gooseLength = 1;
constexpr fp gooseWeight = 4.53592;

constexpr fp gooseToDragonMultiplier = enderDragonHitboxSize.getX() / gooseLength;

constexpr fp enderDragonWeight = gooseWeight * (math::squared(gooseToDragonMultiplier) * gooseToDragonMultiplier);
constexpr fp enderDragonVolume = enderDragonWeight * 0.008;



constexpr fp enderDragonBodyAttackDamage = 5;
constexpr fp enderDragonHeadAttackDamage = 10;
constexpr fp enderDragonFlyingSpeed = 30.2;
constexpr fp enderDragonDamage = 10;
constexpr fp enderDragonMaxAdditionalUpwardsKnockbackForce = 0x20;

struct enderDragon : public mob
{
	enderDragon(dimension* dimensionIn, cvec2& position);
	bodyPart2D* createDragonLeg(cvec2& attachmentPoint, crectangle2& upperLegTextureRect, cfp& upperLegAngle, crectangle2& lowerLegTextureRect, cfp& lowerLegAngle, crectangle2& footTextureRect, cfp& footAngle);
	static void createSpine(bodyPart2D* attachTo, bodyPart2D** spinePtr, cint spineCount);

	virtual void render(const gameRenderData& targetData) const override;
	virtual void tick() override;
	virtual bool addDamageSource(cfp& damage, std::shared_ptr<damageSource> source) override;
	virtual void onDeath() override;
	virtual ~enderDragon() override;

	virtual void updateBodyParts() const override;

	//head = mainbodypart

	bodyPart2D* body = nullptr;

	//[0] = front vertebra
	bodyPart2D* neckSpines[enderDragonNeckSpineCount] = {};
	bodyPart2D* tailSpines[enderDragonTailSpineCount] = {};

	bodyPart2D* lowerJaw = nullptr;

	bodyPart2D* rightInnerWingTop = nullptr;
	bodyPart2D* leftInnerWingTop = nullptr;
	bodyPart2D* rightOuterWingTop = nullptr;
	bodyPart2D* leftOuterWingTop = nullptr;
	bodyPart2D* rightOuterWingSide = nullptr;
	bodyPart2D* leftOuterWingSide = nullptr;

	dragonPhase currentPhase = dragonPhase::noAI;

	//set to true when the player breaks an end crystal.
	//set to false again when the player has got a fireball shot at them
	bool endCrystalBroken = false;

	//the uuid of the nearest end crystal
	uuid uuidHealingFrom = uuid();

	int deathAnimationTicks = 0;
	//x = position, y = size, z = length
	vec3* dyingBeams = nullptr;
};
