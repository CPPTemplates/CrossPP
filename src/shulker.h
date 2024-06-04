#pragma once
#include "mob.h"

//shulker
constexpr rectangle2 shulkerHeadTextureRect = crectangle2(0, 0, 6, 6);
constexpr rectangle2 shulkerBottomShellTextureRect = crectangle2(0, 12, 16, 8);
constexpr rectangle2 shulkerTopShellTextureRect = crectangle2(0, 36, 16, 12);

constexpr vec2 shulkerHitboxSize = vec2(1);

constexpr rectangle2 shulkerRelativeHitbox = crectangle2(shulkerHitboxSize * -0.5, shulkerHitboxSize);

constexpr fp shulkerTextureScale = shulkerHitboxSize.getY() / (fp)0x20;

constexpr vec2 shulkerHeadSize = cvec2(shulkerHeadTextureRect.size) * shulkerTextureScale;
constexpr vec2 shulkerBottomShellSize = cvec2(shulkerBottomShellTextureRect.size) * shulkerTextureScale;
constexpr vec2 shulkerTopShellSize = cvec2(shulkerTopShellTextureRect.size) * shulkerTextureScale;

struct shulker : public mob
{
	colorID shulkerColor = colorID();
	directionID attachmentDirection = directionID::negativeY;
	shulker(dimension* dimensionIn, cvec2& position);

	bodyPart2D* bottomShell = nullptr;
	bodyPart2D* topShell = nullptr;

	void teleportToAttachableBlock();

	virtual void updateBodyParts() const override;
	virtual fp getGravityForce() const override;
	virtual std::vector<vec3> getFrictions() const override;
	bool attached() const;
	bool canAttachTo(cveci2& testPosition, const directionID& testDirection) const;

	void addSpeed(cvec2& additionalSpeed);
};