#pragma once
#include "math/graphics/brush/brushes/solidBrush.h"
#include "math/graphics/color/color.h"
typedef solidBrush<color, vec2> solidColorBrush;
namespace brushes
{
	extern solidColorBrush black;
	extern solidColorBrush white;
	extern solidColorBrush red;
	extern solidColorBrush green;
	extern solidColorBrush blue;
	extern solidColorBrush cyan;
	extern solidColorBrush orange;
	extern solidColorBrush transparent;
}