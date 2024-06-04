#include "math/mathFunctions.h"
#pragma once

struct swingSynchronizer
{
	constexpr swingSynchronizer() {}
	constexpr swingSynchronizer(cfp& swingDuration, cfp& minAngle, cfp& maxAngle, cfp& offset = 0) :swingDuration(swingDuration), minAngle(minAngle), maxAngle(maxAngle), offset(offset) {}

	//the duration of a single swing (back and forth)
	fp swingDuration = 0;

	//the minimum angle of a swing
	fp minAngle = 0;

	//the maximum angle of a swing
	fp maxAngle = 0;

	//offset in timepoints
	fp offset = 0;

	//for rendering
	fp getSwingAngle(cfp& timePoint) const;

	//for sound synchronisation
	bool maximumBetween(cfp& timePoint1, cfp& timePoint2) const noexcept;
};