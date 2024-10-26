#include "math/graphics/color/color.h"
#include "include/math/graphics/color/colorFunctions.h"
constexpr color waterColor = color(hexToColor(0x050533), color::halfMaxValue);//50 percent transparency
constexpr color lavaColor = color(hexToColor(0xd16018), (colorChannel)(color::maxValue * 0.9));//10 percent transparency
