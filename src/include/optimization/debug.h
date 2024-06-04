#pragma once
#include <string>
//#include "windowsIncluder.h"

//writes to output window
//inline void output(std::wstring s)
//{
//	OutputDebugString(s.c_str());
//}

#ifdef NDEBUG //release
constexpr bool isDebugging = false;
#else
//constexpr bool isDebugging = false;//for debugging purposes
constexpr bool isDebugging = true;
#endif // _DEBUG
