#pragma once
#include "lightLevelID.h"
enum class levelID : int
{
	light,
	powerLevel = ((int)light + (int)lightLevelID::count),
	count
};