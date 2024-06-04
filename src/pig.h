#pragma once
#include "fourLeggedPassiveMob.h"

//pig
constexpr rectangle2 pigBodyTextureRect = crectangle2(46, 0, 8, 16);
constexpr rectangle2 pigFrontRightLegTextureRect = crectangle2(0, 6, 4, 6);
constexpr rectangle2 pigFrontLeftLegTextureRect = crectangle2(8, 6, 4, 6);
constexpr rectangle2 pigHeadTextureRect = crectangle2(0, 16, 8, 8);
constexpr fp pigHeadTextureOffset = 2;

constexpr vec2 pigHitboxSize = cvec2(0.9);

constexpr fp pigTextureScale = pigHitboxSize.getY() / (fp)(pigBodyTextureRect.size.getX() + pigFrontRightLegTextureRect.size.getY() + pigHeadTextureOffset);

constexpr fp pigHeadOffset = pigTextureScale * pigHeadTextureOffset;
constexpr fp pigLegOffset = pigTextureScale * 1;

constexpr vec2 pigBodySize = cvec2(pigBodyTextureRect.size.getY() * pigTextureScale, pigBodyTextureRect.size.getX() * pigTextureScale);
constexpr vec2 pigBodyRotationCentre = cvec2(pigBodySize.getX() * 0.5, 0);

constexpr vec2 pigLegSize = cvec2(pigFrontRightLegTextureRect.size.getX() * pigTextureScale, pigFrontRightLegTextureRect.size.getY() * pigTextureScale);
constexpr vec2 pigLegRotationCentre = cvec2(pigLegSize.getX() * 0.5, pigLegSize.getY());

constexpr vec2 pigFrontLegOffset = cvec2(pigBodySize.getX() * 0.5 - pigLegSize.getX() + pigLegRotationCentre.getX() - pigLegOffset, 0);
constexpr vec2 pigBackLegOffset = cvec2(-pigBodySize.getX() * 0.5 + pigLegRotationCentre.getX() - pigLegOffset, 0);
constexpr vec2 pigHeadSize = cvec2(pigHeadTextureRect.size.getX(), pigHeadTextureRect.size.getY()) * pigTextureScale;
constexpr vec2 pigHeadRotationCentre = cvec2(0, pigHeadSize.getY() * 0.5);


struct pig : public fourLeggedPassiveMob
{
	pig(dimension* dimensionIn, cvec2& position);
};