#pragma once
#include "fourLeggedPassiveMob.h"
//cow
constexpr rectangle2 cowBodyTextureRect = crectangle2(40, 0, 10, 18);
constexpr rectangle2 cowFrontRightLegTextureRect = crectangle2(0, 0, 4, 12);
constexpr rectangle2 cowFrontLeftLegTextureRect = crectangle2(8, 0, 4, 12);
constexpr rectangle2 cowHeadTextureRect = crectangle2(0, 18, 6, 8);
constexpr fp cowHeadTextureOffset = 1;

constexpr vec2 cowHitboxSize = cvec2(0.9, 1.4);
constexpr fp cowTextureScale = cowHitboxSize.getY() / (fp)(cowBodyTextureRect.size.getX() + cowFrontRightLegTextureRect.size.getY() + cowHeadTextureOffset);

constexpr fp cowHeadOffset = cowTextureScale * cowHeadTextureOffset;

constexpr vec2 cowBodySize = cvec2(cowBodyTextureRect.size.getY() * cowTextureScale, cowBodyTextureRect.size.getX() * cowTextureScale);
constexpr vec2 cowBodyRotationCentre = cvec2(cowBodySize.getX() * 0.5, 0);

constexpr vec2 cowLegSize = cvec2(cowFrontRightLegTextureRect.size.getX() * cowTextureScale, cowFrontRightLegTextureRect.size.getY() * cowTextureScale);
constexpr vec2 cowLegRotationCentre = cvec2(cowLegSize.getX() * 0.5, cowLegSize.getY());

constexpr vec2 cowFrontLegOffset = cvec2(cowBodySize.getX() * 0.5 - cowLegSize.getX() + cowLegRotationCentre.getX(), 0);
constexpr vec2 cowBackLegOffset = cvec2(-cowBodySize.getX() * 0.5 + cowLegRotationCentre.getX() - cowTextureScale * 0x2, 0);
constexpr vec2 cowHeadSize = cvec2(cowHeadTextureRect.size.getX(), cowHeadTextureRect.size.getY()) * cowTextureScale;
constexpr vec2 cowHeadRotationCentre = cvec2(0, cowHeadSize.getY() * 0.5);

struct cow : public fourLeggedPassiveMob
{
	cow(dimension* dimensionIn, cvec2& position);
};