#pragma once
#include "humanoid.h"
#include "mob.h"
//blaze
constexpr rectangle2 blazeHeadTextureRect = rectangle2(vec2(16, 16), vec2(8));
constexpr rectangle2 blazeRodTextureRect = rectangle2(vec2(4, 6), vec2(2, 8));

constexpr rectangle2 blazeHitbox = humanHitbox;

constexpr size_t blazeRingRodCount = 0x4;
constexpr size_t blazeRingCount = 0x3;
constexpr size_t blazeRodCount = blazeRingCount * blazeRingRodCount;
//the amount of rods per ring that are drawn behind the blaze
constexpr int blazeRingBackRodCount = blazeRingRodCount / 2;
constexpr fp blazePixelSize = humanHeadSize.getX() / blazeHeadTextureRect.getW();

constexpr vec2 blazeHeadSize = vec2(blazeHeadTextureRect.size) * blazePixelSize;
constexpr vec2 blazeRodSize = vec2(blazeRodTextureRect.size) * blazePixelSize;
constexpr fp relativeBlazeRingHeight[blazeRingCount]
{
	blazeHeadSize.getY() * 0.5,
	0,
	-blazeHeadSize.getY()
};
//RADIUS (MAXIMUM DISTANCE)
constexpr fp blazeRingRadius[blazeRingCount]
{
	blazeHeadSize.getX(),
	blazeHeadSize.getX() * 0.7,
	blazeHeadSize.getX() * 0.5,
};

//circles per second
constexpr fp blazeRodSpeed[blazeRingCount]
{
	0.5,
	-0.2,
	0.2
};
struct blaze : public mob
{
	blaze(dimension* dimensionIn, cvec2& position);
	bodyPart2D* rods[blazeRingCount * blazeRingRodCount]{};
	virtual void updateBodyParts() const override;
};