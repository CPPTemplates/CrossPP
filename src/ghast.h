#pragma once
#include "mob.h"
//ghast
constexpr vec2 ghastHitboxSize = vec2(4, 4);
constexpr rectangle2 ghastHitbox = rectangle2(ghastHitboxSize * -0.5, ghastHitboxSize);

constexpr rectangle2 ghastHeadTextureRect = crectangle2(cvec2(), cvec2(0x10));
constexpr rectangle2 ghastLegTextureRect = crectangle2(0, 0x10, 0x2, 14);

constexpr fp ghastPixelSize = ghastHitboxSize.getX() / ghastHeadTextureRect.size.getX();
constexpr vec2 ghastHeadSize = cvec2(ghastHeadTextureRect.size) * ghastPixelSize;
constexpr vec2 ghastMaxLegSize = cvec2(ghastLegTextureRect.size) * ghastPixelSize;

constexpr vec2 ghastHeadRotationCentre = ghastHeadSize * 0.5;

constexpr fp ghastFlyingSpeed = 0x4;

constexpr int ghastLegCount = 0x8;
struct ghast : public mob
{
	ghast(dimension* dimensionIn, cvec2& position);
	virtual void updateBodyParts() const override;
	bodyPart2D* legs[ghastLegCount];
	swingSynchronizer legSynchronizers[ghastLegCount];
};