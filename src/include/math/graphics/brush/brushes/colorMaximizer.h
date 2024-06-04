#pragma once
#include "math/graphics/brush/brush.h"
template <typename brush0Type, typename brush1Type>
struct colorMaximizer final : public colorBrush
{
	typedef std::conditional_t<std::is_same_v<typename brush0Type::inputType, typename brush1Type::inputType>, typename brush0Type::inputType, colorBrush::inputType> inputType;
	const brush0Type &brush0 = nullptr;
	const brush1Type &brush1 = nullptr;
	colorMaximizer(const brush0Type &brush0, const brush1Type &brush1) : brush0(brush0), brush1(brush1) {}
	virtual inline color getValue(const inputType &pos) const
	{
		return color::maximizeColors(brush0.getValue((typename brush0Type::inputType)pos), brush1.getValue((typename brush1Type::inputType)pos));
	}
};