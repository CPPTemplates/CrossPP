
#include "IItemComparable.h"
#include "itemData.h"
#include "tag.h"
IItemComparable* getItemComparableByName(const std::wstring& name)
{
	const itemID& itemListIndex = itemList.getIDByName(name);
	if ((int)itemListIndex != -1)
	{
		return itemList[itemListIndex];
	}
	size_t tagListIndex = getTagListIndexByName(name);
	if (tagListIndex != std::wstring::npos)
	{
		return tagList[tagListIndex];
	}
	return nullptr;
}
//bool IItemComparable::compare(const itemID& itemToCompare) const
//{
//	handleError(std::wstring(L"not implemented"));
//	return false;
//}

//bool IItemComparable::willCompareToAnyItem() const
//{
//	handleError(std::wstring(L"not implemented"));
//	return false;
//}