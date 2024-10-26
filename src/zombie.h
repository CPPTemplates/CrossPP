#pragma once
#include "humanoid.h"
//zombie
constexpr fp zombiePixelSize = humanPixelSize * 0.5;

constexpr rectangle2 zombieBodyTextureRect = crectangle2(16, 32, 4, 12);
constexpr rectangle2 zombieHeadTextureRect = crectangle2(0, 48, 8, 8);
constexpr rectangle2 zombieRightLegTextureRect = crectangle2(0, 32, 4, 12);
constexpr rectangle2 zombieRightArmTextureRect = crectangle2(40, 32, 4, 12);
constexpr rectangle2 zombieLeftLegTextureRect = crectangle2(8, 32, 4, 12);
constexpr rectangle2 zombieLeftArmTextureRect = crectangle2(48, 32, 4, 12);
struct zombie : public humanoid
{
	zombie(dimension* dimensionIn, cvec2& position, const entityID& entityType);
	virtual void serializeValue(nbtSerializer& s) override;
};