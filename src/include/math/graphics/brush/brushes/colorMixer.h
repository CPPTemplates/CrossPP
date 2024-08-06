#pragma once
#include "math/graphics/color/color.h"
#include "math/graphics/brush/brush.h"
#include "math/graphics/color/colorFunctions.h"
template <typename brush0Type, typename brush1Type>
struct colorMixer final : public colorBrush
{
	typedef brush1Type::inputType inputType;
	const brush0Type &topBrush;
	const brush1Type &bottomBrush;

	colorMixer(const brush0Type &topBrush, const brush1Type &bottomBrush) : topBrush(topBrush), bottomBrush(bottomBrush) {}

	// inline static constexpr color getColor(ccolor& topColor, ccolor& bottomColor)
	//{
	//	//static functions
	//	return (topColor.a() == color::maxValue) ?
	//		topColor :
	//		(topColor.a() ? color::transition(topColor, bottomColor) : bottomColor);
	// }

	inline color getValue(const inputType &pos) const
	{
		ccolor &topColor = topBrush.getValue((typename brush0Type::inputType)pos);
		if (topColor.a() == color::maxValue)
		{
			return topColor;
		}
		else
		{
			ccolor &bottomColor = bottomBrush.getValue(pos);
			if (topColor.a())
			{
				return transitionColor(topColor, bottomColor);
			}
			else
			{
				return bottomColor;
			}
		}

		// the bottom color will be optimized away if the topcolor does not have transparency
		// return getColor(topBrush.getValue(pos), bottomBrush.getValue(pos));
	}
};