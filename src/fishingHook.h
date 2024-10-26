#pragma once
#include "throwable.h"
struct fishingHook : public throwable
{
	fishingHook(dimension* dimensionIn, cvec2& position);
};