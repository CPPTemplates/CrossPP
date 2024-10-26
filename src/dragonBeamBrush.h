#pragma once
#include "math/graphics/brush/brush.h"
#include "math/graphics/color/colorFunctions.h"
constexpr color dragonDyingBeamStartColor = colorPalette::white;
constexpr color dragonDyingBeamEndColor = colorPalette::magenta;
template<typename brush0Type>
struct dragonBeamBrush : colorBrush
{
	const brush0Type* backgroundBrush;
	vec2 dragonScreenLocation = cvec2();
	cfp whiteDistanceSquared = 0;
	cfp transparencyDistanceSquared = 0;
	dragonBeamBrush(const brush0Type* backgroundBrush, cvec2& dragonScreenLocation, cfp& whiteDistance, cfp& transparencyDistance) :
		backgroundBrush(backgroundBrush), dragonScreenLocation(dragonScreenLocation), whiteDistanceSquared(math::squared(whiteDistance)), transparencyDistanceSquared(math::squared(transparencyDistance)) {}
	inline color getValue(cvec2& pos) const
	{
		cfp distanceToDragonSquared = (pos - dragonScreenLocation).lengthSquared();
		return lerpColor(
			(distanceToDragonSquared < whiteDistanceSquared) ?
			lerpColor(dragonDyingBeamStartColor, dragonDyingBeamEndColor, (distanceToDragonSquared / whiteDistanceSquared)) :
			dragonDyingBeamEndColor
			, backgroundBrush->getValue((typename brush0Type::inputType)pos), math::minimum(distanceToDragonSquared / transparencyDistanceSquared, (fp)1));
	}
};