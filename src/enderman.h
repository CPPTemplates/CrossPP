#pragma once
#include "humanoid.h"
//enderman
constexpr rectangle2 endermanBodyTextureRect = crectangle2(32, 0, 2, 12);
constexpr rectangle2 endermanHeadTextureRect = crectangle2(0, 16, 8, 8);
constexpr rectangle2 endermanRightLegTextureRect = crectangle2(56, 0, 2, 30); //outside facing texture of right leg
constexpr rectangle2 endermanRightArmTextureRect = endermanRightLegTextureRect; //outside facing texture of right arm
constexpr rectangle2 endermanLeftLegTextureRect = crectangle2(60, 0, 2, 30); //inside facing texture of left leg
constexpr rectangle2 endermanLeftArmTextureRect = endermanLeftLegTextureRect; //inside facing texture of left arm

constexpr vec2 endermanHitboxSize = vec2(0.6, 2.9);
constexpr fp endermanPixelSize = endermanHitboxSize.getY() / (endermanHeadTextureRect.size.getY() + endermanBodyTextureRect.size.getY() + endermanRightLegTextureRect.size.getY());
constexpr fp endermanEyeLevel = (2 + 6 + 6) * endermanPixelSize;

constexpr vec2 endermanBodySize = vec2(endermanBodyTextureRect.size) * endermanPixelSize;
constexpr vec2 endermanLegSize = vec2(endermanRightLegTextureRect.size) * endermanPixelSize;
constexpr vec2 endermanHeadSize = vec2(endermanHeadTextureRect.size) * endermanPixelSize;
constexpr vec2 endermanArmSize = vec2(endermanRightArmTextureRect.size) * endermanPixelSize;

constexpr rectangle2 endermanHitbox = crectangle2(-endermanHitboxSize.getX() * 0.5, 0, endermanHitboxSize.getX(), endermanHitboxSize.getY());
struct enderman : public humanoid
{
	enderman(dimension* dimensionIn, cvec2& position);
	virtual void serializeValue(nbtSerializer& s) override;
	virtual bool addDamageSource(cfp& damage, std::shared_ptr<damageSource> source) override;
};