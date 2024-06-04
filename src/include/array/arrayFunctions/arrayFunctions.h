#pragma once

#include <vector>
#include <numeric>
#include "math/vector/vectn.h"
#include <set>

template <typename First, typename... T>
constexpr bool is_in(First &&first, T &&...t)
{
    return ((first == t) || ...);
}

template <typename t, typename containerType>
constexpr t getSum(const containerType &values)
{
    return std::accumulate(values.begin(), values.end(), t());
}

template <typename t>
constexpr t getAverage(const std::vector<t> &values)
{
    return std::accumulate(values.begin(), values.end(), t()) / values.size();
}

template <typename T, int arraySize>
constexpr int measureSize(T (&)[arraySize])
{
    return arraySize;
}

template <typename listType>
constexpr void deleteAllElements(const listType &list)
{
    for (const auto *const &element : list)
    {
        delete element;
    }
}

template <typename listType, typename t>
constexpr bool arrayContains(const listType &list, const t &value)
{
    return std::find(std::begin(list), std::end(list), value) != std::end(list);
}

template <typename listType, typename t>
auto find(const listType &v, const t &value)
{
    return std::find(std::begin(v), std::end(v), value);
}

template <typename listType, typename t>
size_t indexof(const listType &v, const t &value)
{
    return std::distance(std::begin(v), find(v, value));
}

template <typename t, size_t n>
constexpr void fillAllElements(t (&arrayToFill)[n], const t &value)
{
    std::fill(&arrayToFill[0], &arrayToFill[n], value);
}

template <typename t, size_t n>
constexpr void copyAllElements(const t (&arrayToCopy)[n], t *const &destination)
{
    std::copy(&arrayToCopy[0], &arrayToCopy[n], destination);
}

template <typename sourceType, typename destType>
constexpr void copyAndCast(const sourceType *const &sourceIt, const sourceType *const &endIt, destType *const &destIt)
{
    if constexpr (std::is_same_v<sourceType, destType>)
    {
        std::copy(sourceIt, endIt, destIt);
    }
    else
    {
        std::transform(sourceIt, endIt, destIt,
                       [](const sourceType &value)
                       { return (destType)value; });
    }
}