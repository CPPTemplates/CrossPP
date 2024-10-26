#pragma once
#include "fourLeggedMob.h"

//wolf
constexpr rectangle2 wolfBodyTextureRect = crectangle2(18, 3, 6, 9);
constexpr rectangle2 wolfNeckTextureRect = crectangle2(21, 19, 7, 6);
constexpr rectangle2 wolfTailTextureRect = crectangle2(9, 4, 2, 8);
constexpr rectangle2 wolfRightLegTextureRect = crectangle2(0, 4, 2, 8);
constexpr rectangle2 wolfLeftLegTextureRect = crectangle2(4, 4, 2, 8);

constexpr rectangle2 wolfRightEarTextureRect = crectangle2(16, 15, 1, 2);
constexpr rectangle2 wolfLeftEarTextureRect = crectangle2(19, 15, 1, 2);

constexpr rectangle2 wolfMuzzleTextureRect = crectangle2(0, 15, 4, 3);
constexpr rectangle2 wolfHeadTextureRect = crectangle2(0, 22, 4, 6);


constexpr vec2 wolfHitboxSize = cvec2(0.6, 0.8);
constexpr rectangle2 relativeWolfHitbox = crectangle2(cvec2(wolfHitboxSize.getX() * -0.5, 0), wolfHitboxSize);

constexpr fp wolfHeadExpansion = (wolfHeadTextureRect.getH() - wolfBodyTextureRect.getH()) * 0.5;

constexpr fp wolfTextureScale = wolfHitboxSize.getY() / (wolfRightLegTextureRect.getH() + ((wolfBodyTextureRect.getW() + wolfBodyTextureRect.getW()) * 0.5) + wolfRightEarTextureRect.getH());

constexpr vec2 wolfBodySize = wolfBodyTextureRect.size.switchAxes() * wolfTextureScale;
constexpr vec2 wolfNeckSize = wolfNeckTextureRect.size.switchAxes() * wolfTextureScale;
constexpr vec2 wolfLegSize = wolfRightLegTextureRect.size * wolfTextureScale;
constexpr vec2 wolfEarSize = wolfRightEarTextureRect.size * wolfTextureScale;
constexpr vec2 wolfMuzzleSize = wolfMuzzleTextureRect.size * wolfTextureScale;
constexpr vec2 wolfHeadSize = wolfHeadTextureRect.size * wolfTextureScale;
constexpr vec2 wolfTailSize = wolfTailTextureRect.size.switchAxes() * wolfTextureScale;

//offset from the sides
constexpr fp wolfBackLegOffset = (wolfBodySize.getX() * -0.5) + wolfLegSize.getX();

constexpr fp wolfFrontLegOffset = ((wolfBodySize.getX() * 0.5) + wolfNeckSize.getX()) - wolfLegSize.getX();

struct wolf : public fourLeggedMob
{
	wolf(dimension* dimensionIn, cvec2& position);
};