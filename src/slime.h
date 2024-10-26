#pragma once
#include "mob.h"
//slime
constexpr rectangle2 slimeOuterBodyTextureRect = crectangle2(0, 16, 8, 8);
constexpr rectangle2 slimeInnerBodyTextureRect = crectangle2(0, 4, 6, 6);
constexpr rectangle2 slimeEyeTextureRect = crectangle2(32, 28, 2, 2);

constexpr rectangle2 editedSlimeTextureRect = crectangle2(0, 0, 8, 8);
struct slime : public mob
{
	fp size = 0;
	slime(dimension* dimensionIn, cvec2& position, const entityID& entityType = entityID::slime);
	virtual void updateBodyParts() const override;
	virtual void serializeValue(nbtSerializer& s) override;
	void resize();
	virtual void createBodyParts();
	virtual bool goToPosition(cvec2& destination) override;
	virtual vec2 getJumpPower() const override;
	virtual fp getAttackDamage() const override;
	virtual fp getWeight() const override;
	virtual void onDeath() override;
	virtual fp getMaxHealth() const override;
};