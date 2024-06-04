#pragma once
#include "math/rectangle/rectangletn.h"
#include "constants.h"
#include "math/vector/vectorfunctions.h"
#include "../itemID.h"

constexpr vect2<fsize_t> chunkSize = cvect2<fsize_t>(0x100, 0x100);
//functions
constexpr veci2 getChunkCoordinates(cvec2& worldPos) noexcept {
	return floorVector<int>(worldPos / cvec2(chunkSize));
}

constexpr vect2<fsize_t> halfChunkSize = chunkSize / 2;

constexpr fsize_t chunkArraySize = chunkSize.volume();

//chunk blocks to 2d: 0x10000
constexpr vect3<fsize_t> minecraftChunkSize = vect3<fsize_t>(0x10, 0x100, 0x10);
constexpr fsize_t minecraftChunkLayerSize = minecraftChunkSize.getX() * minecraftChunkSize.getX();
constexpr fsize_t minecraftChunkArraySize = minecraftChunkSize.getY() * minecraftChunkLayerSize;

constexpr int averageMinecraftHeight = 0x40;
constexpr fsize_t minecraftSubChunkHeight = minecraftChunkSize.getX();
constexpr int vanillaChunkBlockCount = minecraftChunkLayerSize * averageMinecraftHeight;
constexpr int averageChunkBlockCountUnderGround = chunkArraySize;//in an endless world, the average amount of blocks underground = filled
constexpr int spawnRate = (int)chunkSize.volume() / 0x1000;

//you will have to stripmine the same amount of time to find ores
//stripmine: 4 blocks exposed per 2 blocks mined instead of 8
constexpr fp chunkOreMultiplier = (averageChunkBlockCountUnderGround / (fp)vanillaChunkBlockCount) * 2;

constexpr int ticksPerMinecraftSubChunk = 3;

constexpr fp defaultRandomTickSpeed = (fp)(chunkSize.getX() * chunkSize.getY() * ticksPerMinecraftSubChunk) / (fp)(minecraftChunkSize.getX() * minecraftChunkSize.getZ() * minecraftSubChunkHeight);

//the average amount of ticks until a block recieves a random tick
constexpr fp defaultTicksPerRandomTick = chunkArraySize / defaultRandomTickSpeed;

constexpr rectangle2 endPortalBlockRect = crectangle2(0, 0, 1, 13 / (fp)blockTextureSize);
constexpr vec2 endPortalFrameEyeSize = cvec2(0x8 / (fp)blockTextureSize, 1 - endPortalBlockRect.size.getY());
constexpr rectangle2 endPortalFrameEyeBlockRect = crectangle2((1 - endPortalFrameEyeSize.getX()) * 0.5, endPortalBlockRect.size.getY(), endPortalFrameEyeSize.getX(), endPortalFrameEyeSize.getY());

constexpr vec2 arrowHitboxSize = cvec2(0.1);
constexpr rectangle2 relativeArrowHitbox = crectangle2(arrowHitboxSize * -0.5, arrowHitboxSize);
constexpr vec2 arrowRenderSize = cvec2(0.5);
constexpr rectangle2 arrowTextureRect = crectangle2(cvec2(0, 27), cvec2(0x10, 5));
constexpr fp arrowPixelSize = arrowRenderSize.getX() / arrowTextureRect.size.getX();
constexpr vec2 arrowSize = cvec2(arrowTextureRect.size) * arrowPixelSize;

//hotbar
constexpr rectangle2 hotbarTextureRect = crectangle2(1, 235, hotbarSpacing * StandardInventoryColumnCount, hotbarSpacing);
constexpr rectangle2 selectorTextureRect = crectangle2(2, 212, hotbarSpacing, hotbarSpacing);

//rails

constexpr rectangle2 middleTextureRect = crectangle2(0, 0, 0x10, 1);
constexpr rectangle2 railsTextureRect = crectangle2(0, 0, 0x10, 4);
constexpr rectangle2 sleepersTextureRect = crectangle2(0, 0, 0x10, 2);

constexpr fp railsHeight = railsTextureRect.size.getY() / (fp)blockTextureSize;
constexpr fp sleepersHeight = sleepersTextureRect.size.getY() / (fp)blockTextureSize;

constexpr vec2 humanJumpPower = vec2(9, 8.4);
constexpr vec2 humanHitboxSize = vec2(0.6, 1.8);
constexpr vec2 legBrakeSpeed = vec2(humanWalkingSpeed, 15);

constexpr veci2 dropperSlotContainerSize = veci2(3);

constexpr rectangle2 blockTextureRect = crectangle2(cvec2(), cvec2(blockTextureSize));
constexpr vec2 boatHitboxSize = cvec2(1.375, 0.565);
constexpr vec2 boatRotationCentre = cvec2(boatHitboxSize.getX() * 0.5, 0);

constexpr rectangle2 torchTextureRect = crectangle2(7, 0, 2, 10);

constexpr vec2 torchSize = cvec2(torchTextureRect.size) / blockTextureSize;

//bed
constexpr rectangle2 bedFrontTextureRect = crectangle2(0, 42, 6, 16);
constexpr rectangle2 bedBackTextureRect = crectangle2(0, 20, 6, 16);
constexpr rectangle2 bedLegTextureRect = crectangle2(50, 40, 4, 3);
constexpr fp bedHeightPixels = (bedLegTextureRect.size.getY() + bedFrontTextureRect.size.getX());
constexpr fp bedHeight = bedHeightPixels / (fp)blockTextureSize;
constexpr int bedBottomThicknessPixels = 2;
constexpr fp bedHitboxHeight = (bedLegTextureRect.getH() + bedBottomThicknessPixels) / (fp)blockTextureSize;

constexpr vec2 measureStringMaximumSize = cvec2(0x200, 0x200);

constexpr int fenceConnectionPossibilityCount = 2;
constexpr veci2 fenceConnectionCheckPositions[fenceConnectionPossibilityCount]
{
	cveci2(-1,0),
	cveci2(1,0),
};

//chest
constexpr rectangle2 chestBlockRect = crectangle2(chestPixelPadding / (fp)blockTextureSize, 0, chestPixelWidth / (fp)blockTextureSize, chestPixelHeight / (fp)blockTextureSize);

constexpr rectangle2 endRodBaseTextureRelativeRect = crectangle2(4.0 / blockTextureSize, 16.0 / blockTextureSize, 8.0 / blockTextureSize, 2.0 / blockTextureSize);
constexpr rectangle2 endRodPoleTextureRelativeRect = crectangle2(0, 2.0 / blockTextureSize, 4.0 / blockTextureSize, 20.0 / blockTextureSize);

constexpr rectangle2 endRodBaseRelativeRect = crectangle2((1 - endRodBaseTextureRelativeRect.size.getX()) * 0.5, 0, endRodBaseTextureRelativeRect.size.getX(), endRodBaseTextureRelativeRect.size.getY());
constexpr rectangle2 endRodPoleRelativeRect = crectangle2((1 - endRodPoleTextureRelativeRect.size.getX()) * 0.5, endRodBaseTextureRelativeRect.size.getY(), endRodPoleTextureRelativeRect.size.getX(), endRodPoleTextureRelativeRect.size.getY());

constexpr int stemPlantGrowthStageCount[stemPlantTypeCount]{ 8, 8 };
//it takes one day on average
constexpr fp stemPlantChanceToGrow[stemPlantTypeCount]
{
	defaultTicksPerRandomTick * stemPlantGrowthStageCount[0] / ticksPerDay,
	defaultTicksPerRandomTick * stemPlantGrowthStageCount[1] / ticksPerDay
};