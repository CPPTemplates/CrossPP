#pragma once
#include "interface/inamable.h"
struct noteData : INamable
{
	noteData(const std::wstring& name) : INamable(name) {}
};