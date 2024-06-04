#pragma once
#include "math/vector/vectn.h"
template <typename outputType = int, typename inputType, fsize_t axisCount>
constexpr vectn<outputType, axisCount> floorVector(const vectn<inputType, axisCount> &vec)
{
    vectn<outputType, axisCount> result = vectn<outputType, axisCount>();
    for (fsize_t i = 0; i < axisCount; i++)
    {
        result[i] = (outputType)math::floor<outputType, inputType>(vec[i]);
    }
    return result;
}

template <typename outputType = int, typename inputType, fsize_t axisCount>
constexpr vectn<outputType, axisCount> ceilVector(const vectn<inputType, axisCount> &vec)
{
    vectn<outputType, axisCount> result = vectn<outputType, axisCount>();
    for (fsize_t i = 0; i < axisCount; i++)
    {
        result[i] = (outputType)math::ceil<outputType, inputType>(vec[i]);
    }
    return result;
}

// https://www.omnicalculator.com/math/angle-between-two-vectors
// a and b have to be NORMALIZED
template <typename t>
inline fp angleBetween(const vect2<t> &a, const vect2<t> &b)
{
    return acos(vec2::dot(a, b));
}

// y is greater at the top
template <typename t>
constexpr bool woundClockwise(const vect2<t> &a, const vect2<t> &b, const vect2<t> &c)
{
    // counter-clockwise
    const vect2<t> &dab = b - a;
    const vect2<t> &dac = c - a;
    const t &winding = dab.x * dac.y - dab.y * dac.x;
    return winding < 0;
}

template <typename indexT, typename sizeT, fsize_t n, typename returnIndexType = std::conditional_t<1 < n && sizeof(indexT) < 4, fsize_t, indexT>>
constexpr std::enable_if_t<std::is_integral_v<indexT>, returnIndexType> singleDimensionalIndex(const vectn<indexT, n> &index, const vectn<sizeT, n> &size)
{
    if constexpr (n == 1)
    {
        return index.axis[0];
    }
    else if constexpr (n == 2)
    {
        return index.axis[0] + index.axis[1] * size.axis[0];
    }
    else
    {
        returnIndexType sdIndex = index[n - 1];
        for (fsize_t i = n - 2;; i--)
        {
            index *= size.axis[i];
            index += index.axis[i];
            if (i == 0)
            {
                return sdIndex;
            }
        }
        // can't come here
        // assumeInRelease(false);
    }
}

template <typename t, fsize_t n>
constexpr bool inBounds(const vectn<t, n> &index, const vectn<t, n> &size)
{
    for (auto it : std::views::zip(index, size))
    {
        if (it.template val<0>() < 0 || it.template val<0>() >= it.template val<1>())
        {
            return false;
        }
    }
    return true;
}