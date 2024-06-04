#include "constants.h"
#include "math/keyframe.h"

constexpr lightLevel maxLightLevel = 0x100;
constexpr lightLevel glowInTheDarkLightLevel = (lightLevel)(maxLightLevel * 0.1);
constexpr lightLevel moodyLightLevel = (lightLevel)(maxLightLevel * 0.5);
constexpr lightLevel brightLightLevel = (lightLevel)(maxLightLevel * 0.7);
constexpr lightLevel hostileMobSpawnTreshold = glowInTheDarkLightLevel;

constexpr int moonLightLevel = glowInTheDarkLightLevel;
#pragma once
lightLevel getVisibleSunLightLevel(const lightLevel& realSunLightLevel);
const transition<fp> timeToLightLevel = transition<fp>(std::vector<keyFrame<fp>>({
	keyFrame<fp>(dayEnd,maxLightLevel),
	keyFrame<fp>(nightStart,moonLightLevel),
	keyFrame<fp>(nightEnd,moonLightLevel),
	keyFrame<fp>(dayStart,maxLightLevel),
	}
));