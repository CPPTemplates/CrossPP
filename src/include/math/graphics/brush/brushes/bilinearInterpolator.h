#pragma once
#include "math/graphics/brush/brush.h"
#include "math/graphics/texture.h"
#include "math/vector/vectorfunctions.h"

struct bilinearInterpolator final :colorBrush
{
	const texture& baseTexture;
	inline color getValue(cvec2& pos) const
	{
		//first interpolate x, then y
		cvect2<fsize_t> pos00 = floorVector<fsize_t>(pos);
		cvect2<fsize_t> pos11 = cvect2<fsize_t>(
			math::minimum((fsize_t)(pos00.x + 1), baseTexture.size.x - 1),
			math::minimum((fsize_t)(pos00.y + 1), baseTexture.size.y - 1)
		);

		const colorf colorsToInterpolate[]
		{
			baseTexture.getValueUnsafe(pos00),
			baseTexture.getValueUnsafe(cvect2<fsize_t>(pos11.x, pos00.y)),
			baseTexture.getValueUnsafe(cvect2<fsize_t>(pos00.x, pos11.y)),
			baseTexture.getValueUnsafe(pos11)
		};

		cvec2& weight = pos - cvec2(pos00);
		return interpolateColor(colorsToInterpolate, weight);
	}
	bilinearInterpolator(const texture& baseTexture) :baseTexture(baseTexture) {}
};