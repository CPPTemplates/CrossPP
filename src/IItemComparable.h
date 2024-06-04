#pragma once
#include "itemID.h"
//comparable to an item
struct IItemComparable
{
	//compares if an item is the same
	virtual bool compare(const itemID& itemToCompare) const = 0;
	//useful to remove any unnecessary tags
	//virtual bool willCompareToAnyItem() const = 0;
};
IItemComparable* getItemComparableByName(const std::wstring& name);