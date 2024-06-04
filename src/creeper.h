#pragma once
#include "fourLeggedMob.h"
constexpr int creeperFuseLength = 30;
//creeper
constexpr rectangle2 creeperBodyTextureRect = crectangle2(28, 0, 4, 12);
constexpr rectangle2 creeperFrontRightLegTextureRect = crectangle2(0, 6, 4, 6);
constexpr rectangle2 creeperFrontLeftLegTextureRect = crectangle2(8, 6, 4, 6);
constexpr rectangle2 creeperHeadTextureRect = crectangle2(0, 16, 8, 8);

constexpr vec2 creeperHitboxSize = cvec2(0.6, 1.7);
constexpr rectangle2 creeperHitbox = crectangle2(creeperHitboxSize.getX() * -0.5, 0, creeperHitboxSize.getX(), creeperHitboxSize.getY());

constexpr fp creeperTextureScale = creeperHitboxSize.getY() / (fp)(creeperBodyTextureRect.size.getY() + creeperFrontRightLegTextureRect.size.getY() + creeperHeadTextureRect.size.getY());

constexpr vec2 creeperBodySize = cvec2(creeperBodyTextureRect.size) * creeperTextureScale;
constexpr vec2 creeperBodyRotationCentre = cvec2(creeperBodySize.getX() * 0.5, 0);

constexpr vec2 creeperLegSize = cvec2(creeperFrontRightLegTextureRect.size.getX() * creeperTextureScale, creeperFrontRightLegTextureRect.size.getY() * creeperTextureScale);
constexpr vec2 creeperFrontLegRotationCentre = cvec2(0, creeperLegSize.getY());
constexpr vec2 creeperBackLegRotationCentre = creeperLegSize;

constexpr vec2 creeperFrontLegOffset = cvec2(creeperBodySize.getX(), 0) - creeperBodyRotationCentre;
constexpr vec2 creeperBackLegOffset = -creeperBodyRotationCentre;

constexpr vec2 creeperHeadSize = cvec2(creeperHeadTextureRect.size) * creeperTextureScale;
constexpr vec2 creeperHeadRotationCentre = cvec2(creeperHeadSize.getX() * 0.5, 0);

struct creeper : public fourLeggedMob
{
	bool powered = false;
	int explosionStrength = 3;
	int ticksUntilFuse = creeperFuseLength;
	bool ignited = false;
	creeper(dimension* dimensionIn, cvec2& position);
	virtual void render(const gameRenderData& targetData) const override;
	virtual void serializeValue(nbtSerializer& s) override;
};