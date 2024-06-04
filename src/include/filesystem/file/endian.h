#pragma once
#include <cstdint>
#include <utility>
#include <bit>

// no pointers, const values and non-fundamental types accepted
template <typename t>
inline constexpr bool is_endian_convertable_v = (!(std::is_pointer_v<t> || std::is_const_v<t> || std::is_same_v<t, void>)&&std::is_fundamental_v<t>) || std::is_enum_v<t>;

inline void invertEndian(byte *ptr, cint &size)
{
	byte *endPtr = ptr + size - 1;
	while (endPtr > ptr)
	{
		std::swap(*ptr++, *endPtr--);
	}
}

template <typename t, typename = std::enable_if_t<is_endian_convertable_v<t>>>
inline void invertEndian(t &value)
{
	if constexpr (sizeof(t) > 1)
	{
		invertEndian((byte *)&value, sizeof(value));
	}
}
