#pragma once
#include "math/graphics/brush/brush.h"
template<typename brush0Type>
struct grayScaleToTransparency final : colorBrush
{
	const brush0Type& grayScaleBrush;
	inline grayScaleToTransparency(const brush0Type& grayScaleBrush) :grayScaleBrush(grayScaleBrush) {}
	inline color getValue(cvec2& pos) const
	{
		//r, g or b does not matter
		const colorChannel& alpha = grayScaleBrush.getValue(pos).r;
		return color(alpha, 0); // g->getValue(pos);//color((byte)(invert ? 0xff - alpha : alpha), transparentColor.r, transparentColor.g, transparentColor.b);
	}
};