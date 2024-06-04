#pragma once
#include "math/graphics/brush/brush.h"
template<typename resultingType, typename inputType>
struct solidBrush final :public brush<resultingType, inputType>
{
	resultingType value = resultingType();
	solidBrush(const resultingType& value) :value(value) {}
	constexpr resultingType getValue(const inputType&) const noexcept
	{
		return value;
	}
};