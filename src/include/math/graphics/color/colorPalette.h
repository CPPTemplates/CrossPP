#pragma once
#include "color.h"
// template <typename t, fsize_t channelCount>
// struct colorPaletteTN
//{
//     typedef colortn<t, channelCount> colorType;
//     inline static constexpr colorType transparent = colorType(0, 0);
//     inline static constexpr colorType black = colorType(0, 0, 0);
//     inline static constexpr colorType red = colorType(colorType::maxValue, 0, 0);
//     inline static constexpr colorType green = colorType(0, colorType::maxValue, 0);
//     inline static constexpr colorType blue = colorType(0, 0, colorType::maxValue);
//     inline static constexpr colorType yellow = colorType(colorType::maxValue, colorType::maxValue, 0);
//     inline static constexpr colorType magenta = colorType(colorType::maxValue, 0, colorType::maxValue);
//     inline static constexpr colorType cyan = colorType(0, colorType::maxValue, colorType::maxValue);
//     inline static constexpr colorType purple = colorType(colorType::halfMaxValue, 0, colorType::maxValue);
//     inline static constexpr colorType white = colorType(colorType::maxValue, colorType::maxValue, colorType::maxValue);
//     inline static constexpr colorType gray = colorType(colorType::halfMaxValue, colorType::halfMaxValue, colorType::halfMaxValue);
//     inline static constexpr colorType orange = colorType(colorType::maxValue, colorType::halfMaxValue, 0);
//     inline static constexpr colorType brown = colorType(colorType::halfMaxValue, colorType::quarterMaxValue, colorType::quarterMaxValue);
// };
// typedef colorPaletteTN<colorChannel, defaultColorChannelCount> colorPalette;
//namespace colorPalette
//{
//    template <typename colorType = color>
//    constexpr colorType transparent = colorType(0, 0);
//    template <typename colorType = color>
//    constexpr colorType black = colorType(0, 0, 0);
//    template <typename colorType = color>
//    constexpr colorType red = colorType(colorType::maxValue, 0, 0);
//    template <typename colorType = color>
//    constexpr colorType green = colorType(0, colorType::maxValue, 0);
//    template <typename colorType = color>
//    constexpr colorType blue = colorType(0, 0, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType yellow = colorType(colorType::maxValue, colorType::maxValue, 0);
//    template <typename colorType = color>
//    constexpr colorType magenta = colorType(colorType::maxValue, 0, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType cyan = colorType(0, colorType::maxValue, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType purple = colorType(colorType::halfMaxValue, 0, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType white = colorType(colorType::maxValue, colorType::maxValue, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType gray = colorType(colorType::halfMaxValue, colorType::halfMaxValue, colorType::halfMaxValue);
//    template <typename colorType = color>
//    constexpr colorType orange = colorType(colorType::maxValue, colorType::halfMaxValue, 0);
//    template <typename colorType = color>
//    constexpr colorType brown = colorType(colorType::halfMaxValue, colorType::quarterMaxValue, colorType::quarterMaxValue);
//
//}

namespace colorPalette
{
    
    constexpr color transparent = color(0, 0);
    
    constexpr color black = color(0, 0, 0);
    
    constexpr color red = color(color::maxValue, 0, 0);
    
    constexpr color green = color(0, color::maxValue, 0);
    
    constexpr color blue = color(0, 0, color::maxValue);
    
    constexpr color yellow = color(color::maxValue, color::maxValue, 0);
    
    constexpr color magenta = color(color::maxValue, 0, color::maxValue);
    
    constexpr color cyan = color(0, color::maxValue, color::maxValue);
    
    constexpr color purple = color(color::halfMaxValue, 0, color::maxValue);
    
    constexpr color white = color(color::maxValue, color::maxValue, color::maxValue);
    
    constexpr color gray = color(color::halfMaxValue, color::halfMaxValue, color::halfMaxValue);
    
    constexpr color orange = color(color::maxValue, color::halfMaxValue, 0);
    
    constexpr color brown = color(color::halfMaxValue, color::quarterMaxValue, color::quarterMaxValue);
//
}