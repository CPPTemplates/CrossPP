#pragma once
#include "math/graphics/brush/brush.h"
struct squareInterpolator final : public colorBrush
{
	colorf cornerColors[4]{};
	inline color getValue(cvec2& pos) const
	{
		return interpolateColor(cornerColors, pos);
	}

	inline squareInterpolator()
	{

	}

	inline squareInterpolator(ccolor cornerColors[4])
	{
		std::copy(&cornerColors[0], &cornerColors[4], this->cornerColors);
	}
};