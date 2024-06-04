#pragma once
#include "math/graphics/brush/brush.h"
template<typename brush0Type>
struct saturator final :public colorBrush
{
	fp addsaturation = 0.3;
	fp addvalue = 0.1;
	const brush0Type& baseBrush;
	saturator(const brush0Type& baseBrush) :baseBrush(baseBrush) {}
	inline color getValue(cvec2& pos) const
	{
		const color original = baseBrush.getValue(pos);
		const colorf& rgbOriginal(original);
		const colorf& hsv = rgb2hsv(rgbOriginal);
		const colorf& saturated = colorf(hsv.h(), math::minimum(hsv.s() + addsaturation, (fp)1.0), math::minimum(hsv.v() + addvalue, (fp)1.0));
		const colorf& rgbSaturated = hsv2rgb(saturated);
		return color(rgbSaturated);
	}
};