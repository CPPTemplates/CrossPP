#pragma once
#include "passiveMob.h"

// chicken
constexpr rectangle2 chickenBodyTextureRect = crectangle2(12, 9, 6, 8);
constexpr rectangle2 chickenUpperLegTextureRect = crectangle2(36, 24, 1, 5);
constexpr rectangle2 chickenLowerLegTextureRect = crectangle2(33, 29, 1, 3);
constexpr rectangle2 chickenHeadTextureRect = crectangle2(7, 23, 3, 6);
constexpr rectangle2 chickenWingTextureRect = crectangle2(31, 9, 6, 4);
constexpr rectangle2 chickenBeakTextureRect = crectangle2(20, 28, 2, 2);
constexpr rectangle2 chickenWattlesTextureRect = crectangle2(18, 24, 2, 2);
constexpr vec2 chickenHitboxSize = cvec2(0.6, 0.8);

constexpr vec2 chickenHeadTextureOffset = cvec2(0x2, 0x4);
constexpr fp chickenTextureScale = chickenHitboxSize.getY() / (fp)(chickenLowerLegTextureRect.size.getX() + chickenUpperLegTextureRect.size.getY() + chickenBodyTextureRect.size.getX() - chickenHeadTextureOffset.getY() + chickenHeadTextureRect.size.getY());

constexpr vec2 chickenHeadOffset = cvec2(chickenHeadTextureOffset) * chickenTextureScale;

constexpr vec2 chickenBodySize = cvec2(chickenBodyTextureRect.size).switchAxes() * chickenTextureScale;
constexpr vec2 chickenBodyRotationCentre = cvec2(chickenBodySize.getX() * 0.5, 0);

constexpr vec2 chickenUpperLegSize = cvec2(chickenUpperLegTextureRect.size) * chickenTextureScale;
constexpr vec2 chickenUpperLegRotationCentre = cvec2(chickenUpperLegSize.getX() * 0.5, chickenUpperLegSize.getY());

constexpr vec2 chickenLowerLegSize = cvec2(chickenLowerLegTextureRect.size).switchAxes() * chickenTextureScale;
constexpr vec2 chickenLowerLegRotationCentre = cvec2(chickenUpperLegSize.getX(), chickenLowerLegSize.getY());

constexpr vec2 chickenHeadSize = cvec2(chickenHeadTextureRect.size) * chickenTextureScale;
constexpr vec2 chickenHeadRotationCentre = chickenHeadOffset;

constexpr vec2 chickenWingSize = cvec2(chickenWingTextureRect.size) * chickenTextureScale;
constexpr vec2 chickenWingRotationCentre = cvec2(chickenWingSize.getX() * 0.5, chickenWingSize.getY());

constexpr vec2 chickenBeakSize = cvec2(chickenBeakTextureRect.size) * chickenTextureScale;
constexpr vec2 chickenBeakRotationCentre = cvec2(0, chickenBeakSize.getY() * 0.5);

constexpr vec2 chickenWattlesSize = cvec2(chickenWattlesTextureRect.size) * chickenTextureScale;
constexpr vec2 chickenWattlesRotationCentre = cvec2(0, chickenWattlesSize.getY());

struct chicken : public passiveMob
{
	int timeUntilNextEgg = 0;
	chicken(dimension *dimensionIn, cvec2 &position);
	bodyPart2D *leftWing = nullptr;
	bodyPart2D *rightWing = nullptr;
	bodyPart2D *leftUpperLeg = nullptr;
	bodyPart2D *rightUpperLeg = nullptr;
	bodyPart2D *leftLowerLeg = nullptr;
	bodyPart2D *rightLowerLeg = nullptr;
	virtual void updateBodyParts() const override;
	virtual void serializeValue(nbtSerializer &s) override;
	virtual fp getGravityForce() const override;
	virtual bool goToPosition(cvec2 &destination) override;
	virtual void tick() override;
	bool addDamageSource(cfp &damage, std::shared_ptr<damageSource> source) override;

		void resetEggLayTime();
};