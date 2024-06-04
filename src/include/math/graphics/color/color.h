#pragma once
#include "math/vector/vectn.h"
#include "globalFunctions.h"
#include <type_traits>

// bgra structure
template <typename t, fsize_t channelCount>
struct colortn : public vectn<t, channelCount>
{
	typedef t channelType;
	typedef const colortn<t, channelCount> ccolort;

	addMemberName(b, this->axis[0])
		addMemberName(g, this->axis[1])
			addMemberName(r, this->axis[2])
				addMemberName(a, this->axis[3])

		// memory alignment for constructors:
		//(r, g, b) -> b, g, r
		//(h, s, v) -> v, s, h
		addMemberName(v, this->axis[0])
			addMemberName(s, this->axis[1])
				addMemberName(h, this->axis[2])

					static constexpr size_t byteSize = sizeof(t) * channelCount;

	static constexpr size_t bitSize = byteSize * ByteToBits;

	static constexpr bool isByteColor = std::is_same_v<std::remove_const_t<t>, byte>;
	static constexpr t maxValue = std::is_integral<t>::value ? (isByteColor ? bytemax : 0x100) : 1;

	static constexpr t halfMaxValue = maxValue / 2;
	static constexpr t quarterMaxValue = maxValue / 4;

	constexpr colortn(const colortn &other) = default;
	constexpr colortn &operator=(const colortn &other) = default;

	constexpr colortn(ccolort &c, const t &a) : vectn<t, channelCount>(c.axis[0], c.axis[1], c.axis[2], a)
	{
	}
	constexpr colortn(const t &a, const t &r, const t &g, const t &b)
	{
		this->a() = a;
		this->r() = r;
		this->g() = g;
		this->b() = b;
	}
	constexpr colortn(const t &r, const t &g, const t &b) : vectn<t, channelCount>(b, g, r) {
		if constexpr(channelCount > 3){
			a() = maxValue;
		}
	}
	constexpr colortn(const t &a, const t &grayscale) : colortn(a, grayscale, grayscale, grayscale) {}
	constexpr colortn(const t &grayscale) : colortn(grayscale, grayscale, grayscale) {}
	constexpr colortn() // dont initialize a to maxvalue, let it be transparent by default
	{
	}

	explicit constexpr colortn(const vectn<t, channelCount> &in) : vectn<t, channelCount>(in)
	{
	}
	// for converting bgr to bgra
	// template <typename = std::enable_if_t<channelCount == 4>>
	// explicit constexpr colortn(const colortn<t, 3 + 4 - channelCount>& in);
	// for converting bgra to bgr
	// template <typename = std::enable_if_t<channelCount == 3>>
	// when channelcount = 3, then the other channelcount = 4
	// do not add explicit, then you'd have to use it like a constructor
	explicit constexpr colortn(const colortn<t, 7 - channelCount> &in) : vectn<t, channelCount>(in)
	{
		if constexpr (channelCount == 4)
		{
			a() = maxValue;
		}
	}

	// https://stackoverflow.com/questions/41011900/equivalent-ternary-operator-for-constexpr-if
	template <typename t2>
	constexpr colortn(const colortn<t2, channelCount> &other)
	{
		for (fsize_t i = 0; i < channelCount; i++)
		{
			constexpr t2 otherMaxValue = colortn<t2, channelCount>::maxValue;
			if constexpr (maxValue == otherMaxValue)
			{
				this->axis[i] = other.axis[i];
			}
			else
			{
				// multiply to guarantee correct result
				if constexpr (std::is_floating_point<t>::value)
				{
					constexpr t multiplier = (maxValue / (t)otherMaxValue);
					this->axis[i] = (t)other.axis[i] * multiplier;
				}
				else if constexpr (std::is_floating_point<t2>::value) // t is not a floating point type, t2 is
				{
					constexpr t2 multiplier = ((t2)maxValue / otherMaxValue);
					this->axis[i] = (t)(other.axis[i] * multiplier);
				}
				else // both not a floating point, we can't make a constexpr multiplier
				{
					this->axis[i] = (t)((other.axis[i] * maxValue) / otherMaxValue);
				}
			}
		}
	}
	/*a((maxValue == colort<t2>::maxValue) ? in.a() : (in.a() * maxValue) / colort<t2>::maxValue),
	r((maxValue == colort<t2>::maxValue) ? in.r() : (in.r() * maxValue) / colort<t2>::maxValue),
	g((maxValue == colort<t2>::maxValue) ? in.g() : (in.g() * maxValue) / colort<t2>::maxValue),
	b((maxValue == colort<t2>::maxValue) ? in.b() : (in.b() * maxValue) / colort<t2>::maxValue)*/
	//{}

	template <typename t2>
	constexpr static t divideByMaxValue(const t2 &toDivide)
	{
		if constexpr (maxValue == 1)
		{
			return toDivide;
		}
		else
		{
			if constexpr (std::is_integral<t2>::value)
			{
				return (t)(toDivide / maxValue);
			}
			else
			{
				constexpr fp inverseMaxValue = 1.0 / maxValue;
				return (t)(toDivide * inverseMaxValue);
			}
		}
	}

	template <typename t2>
	inline static constexpr t2 multiplyByMaxValue(const t2 &toMultiply)
	{
		if constexpr (maxValue != 1)
		{
			return toMultiply * maxValue;
		}
		else
		{
			return toMultiply;
		}
	}

	template <typename t2, typename t3>
	inline static constexpr t multiplyColorChannels(const t2 &channel0, const t3 &channel1)
	{
		return divideByMaxValue(channel0 * channel1);
	}

	inline static constexpr colortn muliplyColors(ccolort &color0, ccolort &color1)
	{
		return ccolort(
			multiplyColorChannels(color0.r(), color1.r()),
			multiplyColorChannels(color0.g(), color1.g()),
			multiplyColorChannels(color0.b(), color1.b()));
	}

	// get the highest value for each channel
	inline static constexpr colortn maximizeColors(ccolort &color1, ccolort &color2)
	{
		// will be converted to << 8
		return ccolort(math::maximum(color1.r(), color2.r()),
					   math::maximum(color1.g(), color2.g()),
					   math::maximum(color1.b(), color2.b()));
	}
};
typedef byte colorChannel;

constexpr fsize_t bgraColorChannelCount = 4;
constexpr fsize_t rgbColorChannelCount = 3;

//defaulting to BGRA, because of:
//size: 4 is 100 in binary, so the compiler can optimize more
//alpha support
constexpr fsize_t defaultColorChannelCount = bgraColorChannelCount;

typedef colortn<colorChannel, defaultColorChannelCount> color;
typedef const color ccolor;
typedef colortn<colorChannel, rgbColorChannelCount> colorRGB;

typedef colortn<byte, bgraColorChannelCount> colorb;
typedef const colorb ccolorb;

typedef colortn<fp, bgraColorChannelCount> colorf;
typedef const colorf ccolorf;
