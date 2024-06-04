#pragma once
#include "math/graphics/brush/brush.h"
template<typename brush0Type>
struct vignetteBrush final : colorBrush
{
	fp distanceMultiplier = 0;
	vec2 middle = vec2();
	const brush0Type& baseBrush;
	vignetteBrush(cvec2& middle, cfp& darkenAmountAtMaxDistance, cfp& maxDistance, const brush0Type& baseBrush) :distanceMultiplier((darkenAmountAtMaxDistance* color::maxValue) / math::squared(maxDistance)), middle(middle), baseBrush(baseBrush) {}
	virtual inline color getValue(cvec2& pos) const
	{
		return color::muliplyColors(baseBrush.getValue(pos), color::maxValue - (colorChannel)((pos - middle).lengthSquared() * distanceMultiplier));
	}
};