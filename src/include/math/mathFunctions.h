#pragma once
#include "globalFunctions.h"
#include <climits>
#include <cmath>
#include "optimization/debug.h"
#include <bit>
// #include <stdint.h>

namespace math
{
	constexpr fp PI = 3.14159265359;
	constexpr fp PI2 = PI + PI; // a whole circle
	constexpr fp goldenRatio = 1.618033988749894848204586834;
	constexpr fp degreesToRadians = PI / 180;
	constexpr fp radiansToDegrees = 180 / PI;
	constexpr fp fpepsilon = 0.0001;
	constexpr fp averageSinusHillValue = 2.0 / PI;

	template <typename t>
	constexpr t sqrt(const t& x, const t& curr, const t& prev)
	{
		return curr == prev ? curr : sqrt(x, (t)0.5 * (curr + x / curr), curr);
	}

	template <typename t>
	constexpr t sqrt(const t& x)
	{
		return x >= (t)0 && x < std::numeric_limits<t>::infinity()
				   ? sqrt(x, x, (t)0)
				   : std::numeric_limits<t>::quiet_NaN();
	}
	constexpr fp sqrt2 = sqrt(2.0);

	template <typename t = fsize_t>
	constexpr t getNextPowerOf2Multiplied(const t &n)
	{
		t i = 1;
		for (; i < n; i *= 2)
			;
		return i;
	}
	template <typename t = fsize_t>
	constexpr t getNextPowerOf2(const t &n)
	{
		t power = 1;
		for (t i = 1; i < n; i *= 2)
		{
			power++;
		}
		return power;
	}

	// degrees can be 0, 90, 180 or 270
	constexpr int sinDegrees(cint &degrees)
	{
		switch (degrees)
		{
		case 0:
			return 0;
		case 90:
			return 1;
		case 180:
			return 0;
		case 270:
			return -1;
		default:
			if constexpr (isDebugging)
			{
				throw std::wstring(L"only 0, 90, 180 or 270");
			}
			else
			{
				return 0;
			}
		}
	}
	// degrees can be 0, 90, 180 or 270
	inline int cosDegrees(cint &degrees)
	{
		cint &sinusEquivalentDegrees = degrees + 90;
		return sinDegrees(sinusEquivalentDegrees == 360 ? 0 : sinusEquivalentDegrees);
	}
	template <typename testFunction>
	inline bool isSummitBetween(cfp &min, cfp &max, testFunction func)
	{
		cfp &valueMin = func(min);
		return func(min - fpepsilon) < valueMin && valueMin > func(max);
	}
	inline fp Log(cfp value, cfp base)
	{
		return std::log(value) / std::log(base);
	}
	template <typename t>
	constexpr t squared(const t &value)
	{
		return value * value;
	}

	template <typename t>
	constexpr t absolute(const t &value)
	{
		return value < 0 ? -value : value;
	}

	// https://stackoverflow.com/questions/824118/why-is-floor-so-slow
	template <typename outputType = int, typename inputType>
	constexpr outputType floor(const inputType &x)
	{
		if (std::is_integral_v<outputType>)
		{
			outputType i = (outputType)x; /* truncate */
			return i - (i > x);			  /* convert trunc to floor */
		}
		else
		{
			int i = (int)x;					  /* truncate */
			return (outputType)(i - (i > x)); /* convert trunc to floor */
		}
	}

	template <typename outputType = int, typename inputType>
	constexpr outputType ceil(const inputType &x)
	{
		if (std::is_integral_v<outputType>)
		{
			const outputType &i = (outputType)x;	   /* truncate */
			return i + (outputType)((inputType)i < x); /* convert trunc to floor */
		}
		else
		{
			int i = (int)x;									  /* truncate */
			return (outputType)(i + (int)((inputType)i < x)); /* convert trunc to floor */
		}
	}

	// floors a value to a unit (floor(5,2) will return 4)
	template <typename t>
	constexpr t floor(const t value, const t unit)
	{
		cfp divided = (fp)value / (fp)unit;			// divide by the value
		cfp flooredValue = std::floor(divided); // round towards zero
		return (t)(flooredValue * (fp)unit);		// multiply back, so we basically rounded it down in units of [unit]
	}
	// floors a value to a unit and substracts the input from the result (mod(5,2) will return 1)
	template <typename t>
	constexpr t mod(const t value, const t unit)
	{
		return value - floor(value, unit); // return difference
	}

	// returns a when w = 0
	// returns b when w = 1
	template <typename t>
	constexpr t lerp(const t &a, const t &b, cfp &w)
	{
		return (t)(a + (b - a) * w);
	}

	// 1 0 1 2 4

	template <typename t>
	constexpr t mapValue(const t &in, const t &imin, const t &imax, const t &omin, const t &omax)
	{
		const t &mult = (omax - omin) / (imax - imin);
		const t &plus = omin - (imin * mult);
		return in * mult + plus;
	}

	// creates an exponential curve, steeper at the min side.

	// steepness: 0 =
	template <typename t>
	constexpr t mapValueExponentiallyMinCurve(const t &in, const t &imin, const t &imax, const t &omin, const t &omax, const t &steepness)
	{
		// imin: 1
		// imax: 0
		const t &inRange0To1Inverted = (imax - in) / (imax - imin);

		const t &maxPow = 1;
		const t &minPow = pow(0.5, steepness);

		const t &outputInverted = pow(0.5, inRange0To1Inverted * steepness);

		const t &outputInRange0To1Inverted = (outputInverted - minPow) / (maxPow - minPow);

		// invert
		return omin + (omax - omin) * outputInRange0To1Inverted;
	}

	// creates an exponential curve, steeper at the max side.
	// swapped order
	template <typename t>
	constexpr t mapValueExponentiallyMaxCurve(const t &in, const t &imin, const t &imax, const t &omin, const t &omax, const t &steepness)
	{
		return mapValueExponentiallyMinCurve(in, imax, imin, omax, omin, steepness);
	}

	// to limit a value between bounds
	template <typename t>
	constexpr t clamp(const t &value, const t &min, const t &max)
	{
		return value < min ? min : value > max ? max
											   : value;
	}
	template <typename t>
	constexpr int GetSign(const t &value)
	{
		return value > 0 ? 1 : value < 0 ? -1
										 : 0;
	}

	template <typename t>
	constexpr t powSizeTSimple(const t &value, cfsize_t &power)
	{
		if (power)
		{
			t result = value;
			fsize_t raisedPower = 1;

			while (raisedPower < power)
			{
				fsize_t doubleRaisedPower = raisedPower + raisedPower;

				cbool &fastMethod = doubleRaisedPower <= power;

				result *= fastMethod ? result : value;
				raisedPower = fastMethod ? doubleRaisedPower : raisedPower + 1;
			}
			return result;
		}
		else
		{
			return (t)1;
		}
	}

	// https://stackoverflow.com/questions/2622441/c-integer-floor-function
	inline int FloorDiv(int a, int b)
	{
		if (a < 0 || b < 0)
		{
			ldiv_t res = ldiv(a, b);
			return (res.rem) ? res.quot - 1
							 : res.quot;
		}
		else
		{
			return a / b;
		}
	}

	// https://en.wikipedia.org/wiki/Fast_inverse_square_root
	// DONT CHANGE PRECISION!
	// returns 1 / sqrt(number)
	inline constexpr20 float fastInverseSqrt(float number) noexcept
	{
		constexpr float threehalfs = 1.5F;

		const float &x2 = number * 0.5F;
#if __cplusplus >= 202002L
		std::uint32_t i = std::bit_cast<std::uint32_t>(number);
		i = 0x5f3759df - (i >> 1);
		number = std::bit_cast<float>(i);
#else
		// can be dangerous!
		std::uint32_t *i = reinterpret_cast<std::uint32_t *>(&number);
		*i = 0x5f3759df - (*i >> 1);
		number = *reinterpret_cast<float *>(&i);
#endif
		number *= threehalfs - (x2 * math::squared(number));
		return number;
	}

	// https://www.gamedev.net/forums/topic/704525-3-quick-ways-to-calculate-the-square-root-in-c/
	// returns 1 / sqrt(number) * number
	inline constexpr20 float fastSqrt(const float &number) noexcept
	{
		return number * fastInverseSqrt(number);
	}

	// a + (b - a) * w = c
	//(b - a) * w = c - a
	// w = (c - a)/(b - a)
	template <typename t>
	inline fp getw(const t a, const t b, const t c)
	{
		return (c - a) / (b - a);
	}
	template <typename t>
	constexpr t maximum(const t &left, const t &right)
	{
		return left > right ? left : right;
	}
	template <typename t>
	constexpr t minimum(const t &left, const t &right)
	{
		return left < right ? left : right;
	}

	// same slope = 0
	inline fp calculateAngle(cfp &slope0, cfp &slope1)
	{
		return math::absolute(atan((slope1 - slope0) / ((fp)1 + slope1 * slope0)));
	}
	inline fp Remainder1(cfp &value)
	{
		return value - floor(value);
	}
	template <typename t>
	inline int sign(const t &value)
	{
		return value > 0 ? 1 : value < 0 ? -1
										 : 0;
	}
	// simulates 1 + 2 + 3 + 4 + 5...
	inline fp calculateIterativeAddition(cfp &iterationCount)
	{
		return iterationCount * (iterationCount + 1) * 0.5;
	}

	inline int floorToBase(cint &val, cint &base)
	{
		int floored = 1;
		while (floored * base < val)
		{
			floored *= base;
		}
		return floored;
	}
}
