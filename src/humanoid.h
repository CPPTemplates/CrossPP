#pragma once
#include "mob.h"
#include "slotContainer.h"
#include "math/swingsynchronizer.h"
#include "constants/vectorConstants.h"

//human
constexpr vec2 humanSkinSize = vec2(32);
constexpr rectangle2 humanBodyTextureRect = crectangle2(8, 16, 2, 6);
constexpr rectangle2 humanHeadTextureRect = crectangle2(0, 24, 4, 4);
constexpr rectangle2 humanRightLegTextureRect = crectangle2(0, 16, 2, 6); //outside facing texture of right leg
constexpr rectangle2 humanRightArmTextureRect = crectangle2(20, 16, 2, 6); //outside facing texture of right arm
constexpr rectangle2 humanLeftLegTextureRect = crectangle2(8, 0, 2, 6); //inside facing texture of left leg
constexpr rectangle2 humanLeftArmTextureRect = crectangle2(16, 0, 2, 6); //inside facing texture of left arm

constexpr fp humanPixelSize = humanHitboxSize.getY() / (humanHeadTextureRect.size.getY() + humanBodyTextureRect.size.getY() + humanRightLegTextureRect.size.getY());
constexpr fp humanEyeLevel = (2 + 6 + 6) * humanPixelSize;

constexpr vec2 humanBodySize = vec2(humanBodyTextureRect.size) * humanPixelSize;
constexpr vec2 humanLegSize = vec2(humanRightLegTextureRect.size) * humanPixelSize;
constexpr vec2 humanHeadSize = vec2(humanHeadTextureRect.size) * humanPixelSize;
constexpr vec2 humanArmSize = vec2(humanRightArmTextureRect.size) * humanPixelSize;

constexpr fp humanStandHeight = 1.8, sneakheight = 1.5;//1.65 on bedrock, but from javas perspective
constexpr fp maxhumanhealth = 20;
constexpr int maxhumanfoodlevel = 20;
constexpr fp maxArmor = 20;
constexpr fp cooldownTreshold = 0.8;//you will deal full damage from this point on
constexpr rectangle2 humanHitbox = crectangle2(-humanHitboxSize.getX() * 0.5, 0, humanHitboxSize.getX(), humanHitboxSize.getY());

constexpr int waitTime = 5;//the time in ticks to wait before you actually start breaking a block

//convert seconds to relative arm angle
constexpr swingSynchronizer armSwingSynchronizer = swingSynchronizer(1.0 / swingsPerSecond, -0.4 * math::PI, -0.2 * math::PI);

struct humanoid : public mob, public slotContainer
{
	bodyPart2D* rightArm = nullptr;
	bodyPart2D* leftArm = nullptr;
	bodyPart2D* rightLeg = nullptr;
	bodyPart2D* leftLeg = nullptr;

	bool climbing = false;

	//cannot be null
	itemStack* itemHolding = nullptr;

	rectangularSlotContainer* armorSlots = nullptr;

	fp getTotalCoolDownTime() const override;

	virtual bool addDamageSource(cfp& damage, std::shared_ptr<damageSource> source) override;


	virtual int getDefencePoints() const override;
	int getToughnessPoints();

	fp selectedBlockDamage = 0;
	int timeWaitingToDig = waitTime;
	bool digging = false;
	bool sleeping = false;

	bool wantsToDig = false;

	int bowAnimationTime = 0;

	fp getWeight() const override;

	humanoid(dimension* dimensionIn, cvec2& position, const entityID& entityType);

	void initializeBodyParts(crectangle2& headTextureRect, crectangle2& bodyTextureRect, crectangle2& leftLegTextureRect, crectangle2& rightLegTextureRect, crectangle2& leftArmTextureRect, crectangle2& rightArmTextureRect, cfp& pixelSize = humanPixelSize);

	virtual void updateBodyParts() const override;
	void render(const gameRenderData& targetData) const override;
	void updateSelection() override;
	void resetDigProgress();
	virtual fp getAttackDamage() const override;

	virtual bool substractStack(itemStack& stack) override;
	virtual bool addStack(itemStack& stack) override;

	virtual fp getGravityForce() const override;

	//sound synchronisation:
	//hand swings + block break sounds
	virtual ~humanoid() override;
	virtual void serializeValue(nbtSerializer& s) override;

	virtual void tick() override;

	virtual bool placeBlock(blockID blockToPlace);
	std::vector<vec3> getFrictions() const override;
	void launchItem(const itemID& itemType);

	vec2 getHandPosition() const;

	void decreaseDurability(itemStack& stack, cfp& amount = 1);
	void decreaseDurability(itemStack& stack, cfp& amount, cvec2& soundPositionIfBroken);
	virtual void respawn() override;

};


swingSynchronizer getLegSwingSynchronizer(cfp& legHeight, cfp& maxLegAngle = maxHumanLegAngle);