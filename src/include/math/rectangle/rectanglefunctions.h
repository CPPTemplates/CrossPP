#pragma once
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectorfunctions.h"
#include "math/random/random.h"
// DONT USE THIS TO CONVERT TO PIXEL RECTANGLES; USE CEILRECT
template <typename outputType = int, typename inputType, fsize_t axisCount>
inline rectangletn<outputType, axisCount> floorRectangle(crectangletn<inputType, axisCount> &rect)
{
    const auto &pos0 = floorVector<outputType>(rect.pos0);

    const auto &pos1 = floorVector<outputType>(rect.pos0 + rect.size);

    return crectangletn<outputType, axisCount>(pos0, pos1 - pos0);
}
template <typename outputType = int, typename inputType, fsize_t axisCount>
inline rectangletn<outputType, axisCount> ceilRectangle(crectangletn<inputType, axisCount> &rect)
{
    const auto &pos0 = ceilVector<outputType>(rect.pos0);

    const auto &pos1 = ceilVector<outputType>(rect.pos0 + rect.size);

    return crectangletn<outputType, axisCount>(pos0, pos1 - pos0);
}

inline vec2 getrandomPosition(std::mt19937 &generator, crectangle2 &rect, cvec2 &innerRectangleSize)
{
	return vec2(rect.x + randFp(generator, rect.w - innerRectangleSize.x), rect.y + randFp(generator, rect.h - innerRectangleSize.y));
}

constexpr rectangle2 getAbsoluteRect(crectangle2 &outerRect, crectangle2 &relativeRect)
{
	return rectangle2(
		outerRect.x + relativeRect.x * outerRect.w,
		outerRect.y + relativeRect.y * outerRect.h,
		relativeRect.w * outerRect.w,
		relativeRect.h * outerRect.h);
}
constexpr vec2 getAbsolutePosition(crectangle2 &rect, cvec2 &relativePosition)
{
	return rect.pos0 + relativePosition * rect.size;
}
constexpr vec2 getRelativePosition(crectangle2 &rect, cvec2 &absolutePosition)
{
	return (absolutePosition - rect.pos0) / rect.size;
}
inline vec2 getrandomPosition(std::mt19937 &generator, crectangle2 &rect)
{
	return vec2(rect.x + randFp(generator, rect.w), rect.y + randFp(generator, rect.h));
}

// copies a piece out of an old array into a new array
template <typename t>
inline t *cut(t *const &arr, crectanglet1<size_t> &range)
{
	t *newArray = new t[range.w];
	std::copy(arr + range.x, arr + range.pos1().x, newArray);
	return newArray;
}