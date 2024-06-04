#pragma once
#include "globalFunctions.h"

template<fsize_t precision = 0x10>
struct ep
{
	//true = negative
	bool sign = false;

	int exponent = 0;//negative = .### numbers, positive = ###.

	uint data[precision]{};

	ep()
	{

	}

	template<typename t>
	constexpr uint64_t getMult() const
	{
		return std::is_same<t, float>::value ? math::powSizeTSimple<uint64_t>(FLT_RADIX, FLT_MANT_DIG) :
			(std::is_same<t, double>::value ? math::powSizeTSimple<uint64_t>(DBL_RADIX, DBL_MANT_DIG) :
				math::powSizeTSimple<uint64_t>(LDBL_RADIX, LDBL_MANT_DIG));
	}

	//https://en.wikipedia.org/wiki/Double-precision_floating-point_format
	template<typename t>
	ep(const t& val)
	{
		t withoutExponent = frexp(val, &exponent);


		uint64_t baseVal = withoutExponent * getMult<t>();

		data[0] = baseVal;
		data[1] = baseVal >> 32;
	}

	template<typename t>
	operator t() const 
	{
		

		uint64_t baseVal = (uint64_t(data[1]) << 32) + data[0];

		//cut the value down to the least significant bits

		uint64_t mult = getMult<t>();

		while (baseVal > mult)
		{
			baseVal /= 2;
		}
		t withoutExponent = baseVal / (t)mult;

		//t withoutExponent = (baseVal % mult) / (t)mult;

		return ldexp(withoutExponent, exponent);
	}
};