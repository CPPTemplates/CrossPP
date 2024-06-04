#pragma once
#include "globalFunctions.h"
#include "array/fastlist.h"
#include "interface/idestructable.h"
struct jsonContainer
{
	std::wstring value = std::wstring();
	std::vector<jsonContainer> children = std::vector<jsonContainer>();
	size_t getChildIndex(const std::wstring& value) const;
	jsonContainer getChild(const std::wstring& value) const;
	jsonContainer operator[](const std::wstring& name) const;
};

jsonContainer readJson(std::wstring content);
