#pragma once
#include "math/graphics/brush/brush.h"
template <typename colorFunction>
struct functionPointerBrush final : public colorBrush
{
	functionPointerBrush(const colorFunction&& function) :function(function) {}
	const colorFunction&& function;

	inline color getColor(cvec2& pos) const
	{
		return function(pos);
	}
};