#pragma once
#include "math/graphics/brush/brush.h"
#include "math/graphics/brush/brushes.h"
template<typename brush0Type, typename resultingType = brush0Type::resultingType, typename inputType = brush0Type::inputType>
struct repeatingBrush final : public brush<resultingType, inputType>
{
	//repeats the same brush
	const brush0Type& brushToRepeat;
	inputType repeatSize;
	repeatingBrush(const brush0Type& brushToRepeat, const inputType& repeatSize) :brushToRepeat(brushToRepeat), repeatSize(repeatSize) {}
	repeatingBrush(const brush0Type& textureToRepeat) :brushToRepeat(textureToRepeat), repeatSize(textureToRepeat.getClientRect().size) {}
	inline resultingType getValue(const inputType& pos) const
	{
		if constexpr (std::is_same_v<inputType, vec2>) {
			const inputType& remainderPos = vec2(math::mod(pos.x, repeatSize.x), math::mod(pos.y, repeatSize.y));
			return brushToRepeat.getValue(remainderPos);
		}
		else {
			const inputType& remainderPos = inputType(pos.x % repeatSize.x, pos.y % repeatSize.y);
			return brushToRepeat.getValue(remainderPos);
		}
	}
};

template<typename brush0Type, typename targetType>
inline void fillRepeatingRectangle(crectangle2& rect, const brush0Type& b, cvec2& repeatSize, const targetType& renderTarget)
{
	const auto& repeater = repeatingBrush<brush0Type>(b, repeatSize);
	fillRectangle(renderTarget, ceilRectangle(rect), repeater);
}

template<typename brush0Type, typename targetType>
inline void fillRepeatingTexture(crectangle2& rect, const brush0Type& tex, const targetType& renderTarget)
{
	fillRepeatingRectangle(rect, tex, tex.size, renderTarget);
}

/// <summary>
/// CAUTION! this will multiply the RECTANGLE, not the TEXTURE!
/// </summary>
/// <typeparam name="brush0Type"></typeparam>
/// <typeparam name="targetType"></typeparam>
/// <param name="rect"></param>
/// <param name="transform"></param>
/// <param name="tex"></param>
/// <param name="renderTarget"></param>
template<typename brush0Type, typename targetType>
inline void fillUnTransformedRepeatingTexture(crectangle2& rect, cmat3x3& transform, const brush0Type& b, cvec2& repeatSize, const targetType& renderTarget)
{
	const auto& repeater = repeatingBrush<brush0Type>(b, repeatSize);
	fillTransformedRectangle(renderTarget, rect, transform, repeater);
}