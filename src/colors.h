#include <string>
#include "math/graphics/color/color.h"
#include "colorID.h"
#pragma once
const std::wstring colorNames[(int)colorID::count]
{
	std::wstring(L"black"),
	std::wstring(L"white"),
	std::wstring(L"red"),
	std::wstring(L"orange"),
	std::wstring(L"pink"),
	std::wstring(L"yellow"),
	std::wstring(L"lime"),
	std::wstring(L"green"),
	std::wstring(L"light_blue"),
	std::wstring(L"cyan"),
	std::wstring(L"blue"),
	std::wstring(L"magenta"),
	std::wstring(L"purple"),
	std::wstring(L"brown"),
	std::wstring(L"gray"),
	std::wstring(L"light_gray"),
};

constexpr color rgbColorValues[(int)colorID::count]
{
	hexToColor(0x1D1D21),
	hexToColor(0xF9FFFE),
	hexToColor(0xB02E26),
	hexToColor(0xF9801D),
	hexToColor(0xF38BAA),
	hexToColor(0xFED83D),
	hexToColor(0x80C71F),
	hexToColor(0x5E7C16),
	hexToColor(0x3AB3DA),
	hexToColor(0x169C9C),
	hexToColor(0x3C44AA),
	hexToColor(0xC74EBD),
	hexToColor(0x8932B8),
	hexToColor(0x835432),
	hexToColor(0x474F52),
	hexToColor(0x9D9D97),
};

//�# <-heximal value
constexpr color rgbFormatColors[(int)colorID::count]
{
	hexToColor(0xAA0000),
	hexToColor(0x000000),
	hexToColor(0x0000AA),
	hexToColor(0x00AA00),
	hexToColor(0x00AAAA),
	hexToColor(0xAA00AA),
	hexToColor(0xFFAA00),
	hexToColor(0xAAAAAA),
	hexToColor(0x555555),
	hexToColor(0x5555FF),
	hexToColor(0x55FF55),
	hexToColor(0x55FFFF),
	hexToColor(0xFF5555),
	hexToColor(0xFF55FF),
	hexToColor(0xFFFF55),
	hexToColor(0xFFFFFF),
};
