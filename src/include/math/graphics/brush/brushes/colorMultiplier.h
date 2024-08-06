#pragma once
#include "math/graphics/brush/brush.h"
#include "math/graphics/brush/brushes/solidColorBrush.h"
template <typename brush0Type, typename brush1Type>
struct colorMultiplier final : public colorBrush
{
	const brush0Type &baseBrush;
	const brush1Type &multColors;
	colorMultiplier(const brush0Type &baseBrush, const brush1Type &multColors) : baseBrush(baseBrush), multColors(multColors) {}
	inline color getValue(cvec2 &pos) const
	{
		const color &original = baseBrush.getValue((typename brush0Type::inputType)pos);
		//optimization for 0 alpha
		if (original.a() == 0)
			return original;
		const color &multiplyWith = multColors.getValue(pos);
		color multiplied = color::muliplyColors(original, multiplyWith);
		multiplied.a() = color::multiplyColorChannels(original.a(), multiplyWith.a());
		return multiplied;
	}
};

template <typename brush0Type, typename brush1Type, typename targetType>
inline void multiplyRectangle(crectangle2 &rect, const brush0Type &baseBrush, const brush1Type &multiplyColorsWith, const targetType &renderTarget)
{
	const auto &multiplier = colorMultiplier<brush0Type, brush1Type>(baseBrush, multiplyColorsWith);
	fillRectangle(renderTarget, rect, multiplier);
}
template <typename brush0Type, typename targetType>
inline void multiplyRectangle(crectangle2 &rect, const brush0Type &multiplyColorsWith, const targetType &renderTarget)
{
	multiplyRectangle(rect, renderTarget, multiplyColorsWith, renderTarget);
}

template <typename targetType>
inline void multiplyRectangle(crectangle2 &rect, const color &multiplyColorsWith, const targetType &renderTarget)
{
	const solidColorBrush b = solidColorBrush(multiplyColorsWith);
	multiplyRectangle(rect, b, renderTarget);
}