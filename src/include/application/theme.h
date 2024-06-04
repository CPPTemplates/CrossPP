#pragma once
#include "math/graphics/brush/font/font.h"
constexpr int defaultBorderSize = 1;
constexpr color defaultBorderColor = colorPalette::gray;
constexpr veci2 buttonSize = cveci2(0x200, (int)defaultFontSize + defaultBorderSize * 2);
constexpr int buttonMargin = 5;
struct theme
{
	theme(baseFont* font = nullptr, int borderSize = defaultBorderSize, color backGroundColor = colorPalette::black, color borderColor = defaultBorderColor) :
		borderSize(borderSize), backGroundColor(backGroundColor), borderColor(borderColor), font(font) {}
	int borderSize = 1;
	color backGroundColor = colorPalette::black;
	color borderColor = defaultBorderColor;
	baseFont* font = nullptr;
};