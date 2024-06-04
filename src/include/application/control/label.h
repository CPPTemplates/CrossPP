#include "control.h"

#pragma once
struct label :public control
{
	label(const std::wstring& text = std::wstring());
};