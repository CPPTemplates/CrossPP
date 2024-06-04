#pragma once
#include "math/graphics/brush/brush.h"
template<typename brush0Type, typename brush1Type>
struct alphaMask final :public colorBrush
{
	const brush0Type& alphaTex = nullptr;
	const brush1Type& baseBrush = nullptr;

	alphaMask(const brush0Type& AlphaTex, const brush1Type& baseBrush) :alphaTex(AlphaTex), baseBrush(baseBrush) {}
	inline color getValue(cvec2& pos) const
	{
		const color c = baseBrush.getValue(pos);
		return color(c, alphaTex.getValue(pos).a());
	}

};