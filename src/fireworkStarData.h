#pragma once
#include "itemTag.h"
#include "fireworkShapeID.h"
#include "math/graphics/color/color.h"
struct fireWorkStarData : itemTag
{
	bool flicker = false;
	bool trail = false;
	fireworkShapeID shape = fireworkShapeID();

	std::vector<colorb> colors = std::vector<colorb>();
	std::vector<colorb> fadeColors = std::vector<colorb>();
	virtual void serializeValue(nbtSerializer& s) override;
};