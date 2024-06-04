#pragma once
#include <sstream>
// #include "windowsIncluder.h"
template <typename t>
inline bool convertTo(const std::wstring &s, t &value)
{
	if constexpr (std::is_same<t, std::wstring>::value)
	{
		csize_t &start = (s.length() && s[0] == L'\"') ? 1 : 0;
		csize_t &end = (s.length() && s[s.length() - 1] == L'\"') ? (s.length() - 1) : s.length();
		value = s.substr(start, end - start);
		return true;
	}
	else
	{
		if (s.size())
		{
			if constexpr (std::is_integral<t>::value)
			{
				long l;
				if (convertToLong(s, l))
				{
					value = (t)l;
					return true;
				}
			}
			else if constexpr (std::is_floating_point<t>::value)
			{
				fp d;
				if (convertToDouble(s, d))
				{
					value = (t)d;
					return true;
				}
			}
		}
	}
	return false;
}
template <typename tFrom, typename tTo>
inline tTo convertAnyType(const tFrom &value)
{
	if constexpr (std::is_arithmetic<tFrom>::value && std::is_arithmetic<tTo>::value)
	{
		return (tTo)value;
	}
	else if constexpr (std::is_same<tFrom, tTo>::value)
	{
		return value;
	}
	else if constexpr (std::is_same<tTo, std::wstring>::value)
	{
		if constexpr (std::is_pointer<tFrom>::value)
		{
			std::wstringstream ss = std::wstringstream();
			ss << (const void *const)value;
			return ss.str();
		}
		else
		{
			return std::to_wstring(value);
		}
	}
	else
	{
		throw "can not convert this type";
	}
}
// https://stackoverflow.com/questions/36270158/avoiding-narrowing-conversions-with-c-type-traits
// template <typename From, typename To>
// concept narrowing_conversion = !requires(From f) { To{f}; };
//
// template <typename From, typename To>
// concept non_narrowing = requires(From f) { To{f}; };

template <typename From, typename To>
concept non_narrowing = std::is_convertible_v<From, To> &&
						((std::is_integral_v<From> && std::is_integral_v<To> && (sizeof(From) <= sizeof(To) && std::numeric_limits<From>::max() <= std::numeric_limits<To>::max())) // both integers
						 || (std::is_floating_point_v<From> && std::is_floating_point_v<To> && sizeof(From) <= sizeof(To))															// both floating point
						 || (std::is_integral_v<From> && std::is_floating_point_v<To> && sizeof(From) < sizeof(To)));																// integer to floating point

// template <typename From, typename To>
// concept non_narrowing = requires() {std::is_same_v<From, To>();};

template <typename From, typename To>
concept narrowing_conversion = !non_narrowing<From, To>;

// constexpr bool res1 = non_narrowing<int, fp>;
// constexpr bool res2 = non_narrowing<fp, int>;
// constexpr bool res3 = std::convertible_to<int, fp>;
//