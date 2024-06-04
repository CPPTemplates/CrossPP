//#include "globalFunctions.h"
#include <string>

#pragma once
enum class axisID
{
	x,
	y,
	z,
	w,
	count
};

const std::wstring axisNames[(size_t)axisID::count]
{
	std::wstring(L"x"),
	std::wstring(L"y"),
	std::wstring(L"z"),
	std::wstring(L"w")
};

typedef const axisID caxisID;