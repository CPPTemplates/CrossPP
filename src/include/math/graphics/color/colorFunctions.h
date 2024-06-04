#pragma once
#include "color.h"
template <typename t, fsize_t channelCount, size_t colorCount>
inline static constexpr colortn<t, channelCount> interpolateColor(const colortn<fp, channelCount> (&colorsToInterpolate)[colorCount], cfp weight[colorCount])
{
    colortn<fp, channelCount> result = colortn<t, channelCount>();
    for (size_t i = 0; i < colorCount; i++)
    {
        result.r() += (colorsToInterpolate[i].r() * weight[i]);
        result.g() += (colorsToInterpolate[i].g() * weight[i]);
        result.b() += (colorsToInterpolate[i].b() * weight[i]);
    }
    result.a() = 1;
    return result;
}

template <fsize_t channelCount>
// https://en.wikipedia.org/wiki/Bilinear_interpolation
inline static constexpr colortn<fp, channelCount> interpolateColor(const colortn<fp, channelCount> (&colorsToInterpolate)[4], cvec2 &weights)
{
    cvec2 &invertedWeigths = (fp)1 - weights;
    cfp weightArray[4]{
        invertedWeigths.x * invertedWeigths.y,
        weights.x * invertedWeigths.y,
        invertedWeigths.x * weights.y,
        weights.x * weights.y,
    };

    return interpolateColor<fp, channelCount, 4>(colorsToInterpolate, weightArray);

    // return colortn<t,channelCount>(
    //	(colorsToInterpolate[0].r() * weightArray[0]) + (colorsToInterpolate[1].r() * weightArray[1]) + (colorsToInterpolate[2].r() * weightArray[2]) + (colorsToInterpolate[3].r() * weightArray[3]),
    //	(colorsToInterpolate[0].g() * weightArray[0]) + (colorsToInterpolate[1].g() * weightArray[1]) + (colorsToInterpolate[2].g() * weightArray[2]) + (colorsToInterpolate[3].g() * weightArray[3]),
    //	(colorsToInterpolate[0].b() * weightArray[0]) + (colorsToInterpolate[1].b() * weightArray[1]) + (colorsToInterpolate[2].b() * weightArray[2]) + (colorsToInterpolate[3].b() * weightArray[3])
    //);
}

template <typename colorType = color>
inline static constexpr colorType hexToColor(cuint &hex)
{
    if constexpr (colorType::maxValue != bytemax)
    {
        constexpr fp multiplier = colorType::maxValue / bytemax;
        return colorType(
            (typename colorType::channelType)((hex / 0x10000) * multiplier),
            (typename colorType::channelType)(((hex / 0x100) & 0xff) * multiplier),
            (typename colorType::channelType)((hex & 0xff) * multiplier));
    }
    else
    {
        return colorType((typename colorType::channelType)(hex / 0x10000), (typename colorType::channelType)((hex / 0x100) & 0xff), (typename colorType::channelType)(hex & 0xff));
    }
}
template <typename colorType = color>
inline static colorType stringToColor(const std::wstring &str)
{
    if (str[0] == L'#')
    {
        long l;
        if (convertToLong(str.substr(1, 6), l, 16))
        {
            return hexToColor(l);
        }
    }
    throw "can't parse the color";
}
// returns the average of 4 colors.
// inline static colortn<t, channelCount> Average(colortn<t,channelCount>& c1, colortn<t,channelCount>& c2, colortn<t,channelCount>& c3, colortn<t,channelCount>& c4)
//{
//	return colortn<t,channelCount>(
//		(c1.a() + c2.a() + c3.a() + c4.a()) / 4,
//		(c1.r() + c2.r() + c3.r() + c4.r()) / 4,
//		(c1.g() + c2.g() + c3.g() + c4.g()) / 4,
//		(c1.b() + c2.b() + c3.b() + c4.b()) / 4
//	);
// }

// returns the average of 4 colors.
template <typename t, fsize_t channelCount>
inline static colortn<t, channelCount> averageColor(const colortn<t, channelCount> &c1, const colortn<t, channelCount> &c2, const colortn<t, channelCount> &c3, const colortn<t, channelCount> &c4)
{
    const auto &totalA = c1.a() + c2.a() + c3.a() + c4.a();
    if (totalA == 0)
    {
        return colortn<t, channelCount>();
    }
    else
    {
        return colortn<t, channelCount>(
            (t)((c1.a() + c2.a() + c3.a() + c4.a()) / 4),
            (t)((c1.r() * c1.a() + c2.r() * c2.a() + c3.r() * c3.a() + c4.r() * c4.a()) / totalA),
            (t)((c1.g() * c1.a() + c2.g() * c2.a() + c3.g() * c3.a() + c4.g() * c4.a()) / totalA),
            (t)((c1.b() * c1.a() + c2.b() * c2.a() + c3.b() * c3.a() + c4.b() * c4.a()) / totalA));
    }
}

// returns c1 on 0 and c2 on 1 and lerps between
template <typename t, fsize_t channelCount>
inline static constexpr colortn<t, channelCount> lerpColor(const colortn<t, channelCount> &c1, const colortn<t, channelCount> &c2, cfp &weight)
{
    return colortn<t, channelCount>(
        c1.r() + (t)(weight * (fp)(c2.r() - c1.r())),
        c1.g() + (t)(weight * (fp)(c2.g() - c1.g())),
        c1.b() + (t)(weight * (fp)(c2.b() - c1.b())));
}
// returns the 'top view' of the colors, above eachother
template <typename t, fsize_t channelCount>
inline static constexpr colortn<t, channelCount> transitionColor(const colortn<t, channelCount> &topcolor, const colortn<t, channelCount> &bottomcolor)
{
    return colortn<t, channelCount>(
        colortn<t, channelCount>::maxValue - colortn<t, channelCount>::multiplyColorChannels(colortn<t, channelCount>::maxValue - bottomcolor.a(), colortn<t, channelCount>::maxValue - topcolor.a()),
        bottomcolor.r() + colortn<t, channelCount>::multiplyColorChannels(topcolor.a(), topcolor.r() - bottomcolor.r()),
        bottomcolor.g() + colortn<t, channelCount>::multiplyColorChannels(topcolor.a(), topcolor.g() - bottomcolor.g()),
        bottomcolor.b() + colortn<t, channelCount>::multiplyColorChannels(topcolor.a(), topcolor.b() - bottomcolor.b()));
}

constexpr uint getUint(const color &c)
{
    return static_cast<uint>(c.axis[0]) + static_cast<uint>(c.axis[1]) * 0x100 + static_cast<uint>(c.axis[2]) * 0x10000 + static_cast<uint>(c.axis[3]) * 0x1000000;
}

colorf rgb2hsv(const colorf &in);
colorf hsv2rgb(const colorf &in);