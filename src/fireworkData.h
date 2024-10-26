#pragma once
#include "itemTag.h"
#include "fireworkStarData.h"
struct fireWorkData : itemTag
{
	std::vector<fireWorkStarData> fireworkStars = std::vector<fireWorkStarData>();
	size_t flightDuration = 0;

	virtual void serializeValue(nbtSerializer& s) override;
};