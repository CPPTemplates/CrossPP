#pragma once
#include <string>
#include <random>
#include "globalFunctions.h"
#include "array/wstringFunctions.h"
#include "math/direction.h"
#include "math/mathFunctions.h"

constexpr int currentFileVersionID = 122;

//forward declarations
//typedefs
typedef int fluidLevel;
typedef int powerLevel;
typedef int lightLevel;
typedef unsigned short port;

//structs
struct itemData;
struct itemStack;
struct block;
struct cropBlock;
struct mob;
struct dimension;
struct world;
struct entity;
struct rectangularSlotContainer;
struct furnaceData;
struct chestData;
struct structureBlockData;
struct jigsawData;
struct idConverter;
struct taskList;
struct dropData;
struct tickableBlockContainer;
struct human;
struct biomeData;


//constants

constexpr wchar_t colorCodeChar = '\xA7';


constexpr int maxNameLetterCount = 0x10;
constexpr int StandardInventoryRowCount = 3;
constexpr int StandardInventoryColumnCount = 9;
constexpr fp netherToOverWorldScale = 0x8;

constexpr int mainEndIslandMaxRadius = 0x100;
constexpr int smallEndIslandsDistance = 0x400;

constexpr int ticksPerRealLifeSecond = 20;

constexpr int ticksPerRealLifeMinute = ticksPerRealLifeSecond * 60;

constexpr fp secondsPerTick = 1.0 / ticksPerRealLifeSecond;
constexpr int microSecondsPerTick = 1000000 / ticksPerRealLifeSecond;

constexpr fp maxFloatingSlotSpeed = 2 * secondsPerTick;
constexpr fp floatingSlotSpeedOnDeath = 10 * secondsPerTick;
constexpr fp maxExperienceSpeed = maxFloatingSlotSpeed;

constexpr fp gravityForce = 1.6;


constexpr powerLevel maxPowerLevel = 0x100;

constexpr fluidLevel maxFluidLevel = 0x100;
constexpr fluidLevel maxSpongeFluidLevel = maxFluidLevel * 0x10;

constexpr fluidLevel bottleFluidLevel = maxFluidLevel / 8;

constexpr int ticksPerHour = 50 * ticksPerRealLifeSecond;
constexpr int ticksPerDay = ticksPerHour * 24;
constexpr int ticksPerMonth = ticksPerDay * 30;
constexpr int ticksPerYear = ticksPerMonth * 12;

//independent variables
constexpr int blockTextureSize = 0x10;


constexpr fp waterWeightPerCubicMeter = 0x10;//1000;
constexpr fp lavaWeightPerCubicMeter = 0x30;// 3000;
constexpr fp ironWeightPerCubicMeter = 7873;
constexpr fp airWeightPerCubicMeter = 1.2;
constexpr fp standardBlockWeightPerCubicMeter = 1000;

constexpr bool checkStructureCollisions = true;

//visible range x: the distance between the screens left and right corner
constexpr fp defaultVisibleRangeXWalk = 0xa;//0x100;//sight [range] blocks far
constexpr fp requiredVisibleRangeXSprint = 0xc;




//lets not be realistic, because it is more fun to encounter more biomes
constexpr int maximumHillHeight = 0x100;//the heighest a hill can be
constexpr int minimumHillHeight = -0x100;//the lowest a hill can be
constexpr fp averageHillHeight = 0x10;

constexpr int sunLightHeight = 0xff;

constexpr int seaLevel = 0;
constexpr int sugarCaneSpawnHeight = seaLevel + 0x4;


constexpr int netherLavaLevel = seaLevel;
constexpr int netherCeilingStart = 0x80;
constexpr int netherCeilingEnd = 0x100;
constexpr int netherLavaOceanBottomLevel = -0x100;
constexpr int netherLavaOceanBedEnd = -0x140;





constexpr fp arrowImpactDamage = 6;
constexpr fp fireBallImpactDamage = 6;


constexpr int cutStoneSlabPixelHeight = blockTextureSize / 0x8;
constexpr fp cutStoneSlabHeight = cutStoneSlabPixelHeight / (fp)cutStoneSlabPixelHeight;

constexpr int obsidianPillarDistance = 43;



constexpr int pistonTopPixelSize = blockTextureSize / 4;
constexpr fp pistonTopSize = pistonTopPixelSize / (fp)blockTextureSize;
constexpr fp pistonArmSize = pistonTopSize;

constexpr fp redstoneWireHeight = 1.0 / 0x10;

constexpr int chestPixelPadding = 1;
constexpr int chestPixelWidth = blockTextureSize - chestPixelPadding * 2;
constexpr int chestPixelHeight = blockTextureSize - chestPixelPadding;

constexpr int floatingSlotDespawnTicks = 5 * ticksPerRealLifeMinute;
constexpr int particleDespawnTicks = ticksPerRealLifeSecond;

constexpr int brewingStandPotionCapacity = 3;
constexpr int brewingBatchTime = 400;
constexpr int blazePowderBrewingBatchCount = 20;//the amount of brewing batches one blaze powder gives

constexpr int experienceCalculation0Treshold = 0x10;
constexpr int experienceCalculation1Treshold = 31;


constexpr int minimalChickenEggTime = 6000;
constexpr int maximalChickenEggTime = 12000;

constexpr int biteInterval = 0x5;
constexpr int slurpInterval = 0x4;
constexpr int foodAnimationTicks = 0x20;
constexpr int normalBiteCount = foodAnimationTicks / biteInterval;
constexpr int driedKelpAnimationTicks = foodAnimationTicks / 2;
constexpr int drinkAnimationTicks = (normalBiteCount + 1) * slurpInterval;



constexpr int netherPortalSearchRangeNether = 0x80;//search in a [range] blocks radius for a nether portal before creating a new one
constexpr int netherPortalSearchRangeOverWorld = netherPortalSearchRangeNether * 8;
constexpr int maxEndPortalSize = 0x10;



constexpr int sunsetDuration = ticksPerRealLifeSecond * 150;
constexpr int dayEnd = ticksPerRealLifeSecond * 385;//light level begins to decrease
constexpr int dayStart = ticksPerRealLifeSecond * 1175;//light level stops to increase
constexpr int nightStart = dayEnd + sunsetDuration;
constexpr int nightEnd = dayStart - sunsetDuration;
constexpr int noon = (dayEnd - (dayStart - ticksPerDay)) / 2;

//constexpr int darkestPoint = (nightEnd + nightStart) / 2;




constexpr fp commandEntityRadius = 0x100;

constexpr fp dragonNoticeDistance = 0x80;

constexpr fp visibleRangeTransitionSpeedPerSecond = 0.9;//in 1 second, it's 10x nearer to the final transition point

constexpr int iconSize = 8;



constexpr fp caveCornerSharpness = 0.4;



constexpr int minimumPortalWidth = 2;
constexpr int minimumPortalHeight = 3;
constexpr int maximumPortalWidth = 0x20;
constexpr int maximumPortalHeight = 0x20;

constexpr int inventorySpacing = 18;//18 pixels before the next item 
constexpr int inventoryItemDisplaySize = 0x10;//16 pixels wide
constexpr int hotbarSpacing = 20;
constexpr int hotbarItemDisplaySize = 15;

constexpr int enchantmentOptionCount = 3;


constexpr int woodTypeCount = 0x8;


constexpr fp armorExpansion = 0.03;

constexpr port defaultPort = 11646;




//1/4 from max power will be half the power
constexpr fp powerStrengthStepCount = 4;
constexpr fp powerTreshold = 1.0 / math::squared(powerStrengthStepCount);

//the maximum angle legs can be at
constexpr fp maxHumanLegAngle = math::degreesToRadians * 30;
constexpr fp maxEndermanLegAngle = math::degreesToRadians * 15;


constexpr fp itemSize = 0.5;//the size of an item in entity form

//the time a mob counts only the highest damage source
constexpr int immunityTime = 10;

//all blocks per seconds
constexpr fp humanSneakingSpeed = 1.31f;
constexpr fp humanWalkingSpeed = 4.317f;
constexpr fp humanSprintingSpeed = 5.612f;
constexpr fp humanFlyingSpeed = 10.92f;
constexpr fp humanSprintFlyingSpeed = 21.78f;
constexpr fp boatSpeed = 0x8;

constexpr fp humanWeightPerCubicMeter = 1010;
constexpr fp boneWeightPerCubicMeter = 1850;

constexpr fp humanWeight = 80;

constexpr fp humanVolume = humanWeight / humanWeightPerCubicMeter;

constexpr fp maxPoweredRailsSpeed = 10;
constexpr fp poweredRailsAcceleration = maxPoweredRailsSpeed * 0.5 * secondsPerTick;


//https://www.google.com/search?rlz=1C1GCEA_enNL863NL863&sxsrf=ALeKk02hhrmwUONaD7B3qcULXPzx5fxRFQ%3A1609352143886&ei=z8PsX4CuNY_ckgXHs7_4Ag&q=skeleton+weight&oq=skeleton+weight&gs_lcp=CgZwc3ktYWIQAzIHCAAQyQMQQzIFCAAQkQIyAggAMgIIADICCAAyBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjIGCAAQFhAeOgQIABBHOgQIIxAnOggIABDJAxCRAjoECAAQQzoKCC4QxwEQowIQQzoECC4QQzoCCC46BAgAEAo6BAguEAo6CgguEMkDEEMQkwI6BwgAEBQQhwJQ__sMWIKSDWDDlA1oAXACeACAAccBiAGqCpIBBDEzLjOYAQCgAQGqAQdnd3Mtd2l6yAEIwAEB&sclient=psy-ab&ved=0ahUKEwjA0er2p_btAhUPrqQKHcfZDy8Q4dUDCA0&uact=5
constexpr fp skeletonWeight = 15;

constexpr fp skeletonVolume = skeletonWeight / boneWeightPerCubicMeter;

//maximum height a player can jump
constexpr fp JumpHeight = 1.25219f;

constexpr fp fenceGateCollisionHeight = JumpHeight + 0.1;

constexpr fp collisionCheckMargin = fenceGateCollisionHeight - 1;

//below 0 it starts freezing
constexpr fp snowTemperatureTreshold = 0;//there will be snow in humid cold areas
constexpr fp snowHeight = 0x20;//there will be snow higher than this


constexpr fp enchantingTableHeight = 0xc / (fp)0x10;
constexpr fp grassPathHeight = 0xf / (fp)0x10;
constexpr fp soulSandHeight = 0xe / (fp)0x10;
constexpr fp unPressedPressurePlateHeight = 0x2 / (fp)0x10;
constexpr fp pressedPressurePlateHeight = 0x1 / (fp)0x10;
constexpr fp unPressedButtonHeight = 0x2 / (fp)0x10;
constexpr fp pressedButtonHeight = 0x1 / (fp)0x10;
constexpr fp buttonWidth = 0x6 / (fp)0x10;


constexpr fp minimalSpawnDistance = requiredVisibleRangeXSprint + 1;
constexpr fp maximumSpawnDistance = 0x80;
constexpr fp randomDespawnDistance = 0x40;

constexpr fp swingsPerSecond = 4;


constexpr fp minimumSlimeSize = 0.5;
constexpr fp maximumSlimeSize = 3;

constexpr int StandardInventoryItemCount = StandardInventoryRowCount * StandardInventoryColumnCount;


//if the mob spawn cooldown is lower than this value,
//mobs will spawn
constexpr int spawnTreshold = 60 * ticksPerRealLifeSecond;
//spawn attempts are doubled
constexpr int doubleSpawnRateTreshold = 0x0;

constexpr int mobCapSpawnCoolDown = 240 * ticksPerRealLifeSecond;
constexpr int mobSpawnCoolDown = 30 * ticksPerRealLifeSecond;

constexpr int maxFood = 20;

constexpr int inventorySlotSize = 0x24;

//constexpr fp hearingRange = 0x20;
//constexpr fp soundLoadRange = hearingRange + 0x40;



//https://gaming.stackexchange.com/questions/178726/what-is-the-terminal-velocity-of-a-sheep
constexpr fp
forceMultiplierInAir = 1,
forceMultiplierOnGround = 1;



constexpr fp frictionPerBlock = 10;
constexpr fp railsFrictionPerBlock = 5;

constexpr int minBlockBreakParticleCount = 0x1;
constexpr int maxBlockBreakParticleCount = 0x4;
constexpr fp maxParticleSpeed = 1 * secondsPerTick;
//same speed at the floating slot that drops; it is just a 'small version'
constexpr fp maxBlockBreakParticleSpeed = maxFloatingSlotSpeed;
constexpr fp maxEatingParticleSpeed = maxFloatingSlotSpeed;



constexpr int openDoorDrawPixels = 3;
constexpr fp openDoorBlockSize = openDoorDrawPixels / (fp)blockTextureSize;

constexpr fp humanArmRange = 4;//4 blocks range
constexpr fp mobSizeMargin = 0x10;//extra range for the arm to check

constexpr fp ridingEntitySearchRadius = 0x8 + mobSizeMargin;
constexpr fp selectedEntitySearchRadius = humanArmRange + mobSizeMargin;
constexpr fp fleeingSearchRadius = 0x10 + mobSizeMargin;

constexpr fp brewingStandBaseHeight = 0x4 / (fp)blockTextureSize;

//
constexpr int maxJigsawLevels = 6;


constexpr fp fencePoleWidth = 0.25;
constexpr fp fencePoleX = (1 - fencePoleWidth) / 2;
constexpr fp fenceConnectionPoleWidth = 0.15;
constexpr int fenceConnectionPoleCount = 2;
constexpr fp fenceConnectionPoleY[fenceConnectionPoleCount]
{
	0.3,
	0.8
};

constexpr fp wallPoleWidth = 0.5;
constexpr fp wallPoleX = (1 - wallPoleWidth) / 2;
constexpr fp wallConnectionHeight = 0.8;

constexpr fp ironBarsPoleWidth = 4.0 / blockTextureSize;
constexpr fp ironBarsPoleX = (1 - ironBarsPoleWidth) / 2;


constexpr directionID standardUpFacingBlockDirection = directionID::positiveY;
constexpr directionID standardSideFacingBlockDirection = directionID::negativeX;

const std::wstring gameName = std::wstring(L"Medieval Survival");
constexpr ull gameAuthNumber = 12969394847103665724ULL;

const std::wstring nbtFileExtension = std::wstring(L".dat");
const std::wstring jsonFileExtension = std::wstring(L".json");


extern std::mt19937 worldRandom;
extern std::mt19937 currentRandom;
//extern microseconds lastTickTimeMicroseconds;
//extern seconds lastTickTime;

//template<typename t>
//std::wstring getClassName(const t& instance);

fp getPowerStrength(const powerLevel& level);

namespace stdFileSystem = std::filesystem;

constexpr int frostedIceMeltTreshold = 0x4;