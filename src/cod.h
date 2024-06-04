#pragma once
#include "mob.h"

//cod
constexpr vec2 codHitboxSize = vec2(0.5, 0.3);
constexpr rectangle2 relativeCodHitbox = rectangle2(codHitboxSize * -0.5, codHitboxSize);

constexpr rectangle2 codHeadTextureRect = crectangle2(16, 25, 3, 4);
constexpr rectangle2 codBodyTextureRect = crectangle2(9, 21, 7, 4);
constexpr rectangle2 codNoseTextureRect = crectangle2(5, 28, 1, 3);
constexpr rectangle2 codDorsalFinsTextureRect = crectangle2(26, 31, 6, 1);
constexpr rectangle2 codCaudalFinTextureRect = crectangle2(26, 21, 4, 4);
constexpr rectangle2 codFrontFinTextureRect = crectangle2(26, 26, 2, 2);

constexpr fp codTextureScale = codHitboxSize.getX() / (codCaudalFinTextureRect.getW() + codBodyTextureRect.getW() + codHeadTextureRect.getW() + codNoseTextureRect.getW());

constexpr vec2 codHeadSize = codHeadTextureRect.size * codTextureScale;
constexpr vec2 codBodySize = codBodyTextureRect.size * codTextureScale;
constexpr vec2 codNoseSize = codNoseTextureRect.size * codTextureScale;
constexpr vec2 codDorsalFinsSize = codDorsalFinsTextureRect.size * codTextureScale;
constexpr vec2 codCaudalFinSize = codCaudalFinTextureRect.size * codTextureScale;
constexpr vec2 codFrontFinSize = codFrontFinTextureRect.size * codTextureScale;

constexpr vec2 codBodyRotationCentre = codBodySize * 0.5;
constexpr vec2 codHeadRotationCentre = cvec2(0, codHeadSize.getY() * 0.5);
constexpr vec2 codNoseRotationCentre = cvec2(0, codNoseSize.getY() * 0.5);
constexpr vec2 codDorsalFinsRotationCentre = cvec2(codDorsalFinsSize.getX() * 0.5, 0);
constexpr vec2 codCaudalFinRotationCentre = cvec2(codCaudalFinSize.getX(), codCaudalFinSize.getY() * 0.5);
constexpr vec2 codFrontFinRotationCentre = cvec2(codFrontFinSize.getX() * 0.5, codFrontFinSize.getY());

constexpr fp codFrontFinTextureOffset = 4;

struct cod : public mob
{
	virtual void updateBodyParts() const override;
	cod(dimension* dimensionIn, cvec2& position);
};