#include "globalFunctions.h"
#include <string>
#pragma once
struct INamable
{
	std::wstring name = std::wstring();
	INamable(const std::wstring& name) :name(name) {}
};