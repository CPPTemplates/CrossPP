#pragma once
#include "IItemComparable.h"
#include "interface/idestructable.h"
#include "array/fastlist.h"
#include "filesystem/filemanager.h"
//represents a list of items that hold this tag
struct tag final :IItemComparable, IDestructable
{
	std::wstring name = std::wstring();
	fastList<IItemComparable*>* taggedComparables = nullptr;
	tag(std::wstring name) :name(name), taggedComparables(new fastList<IItemComparable*>()) {}
	bool hasTaggedItems();
	//copy tagged items to this tag
	virtual bool compare(const itemID& itemToCompare) const override;
	virtual ~tag() override;
};

tag* readTag(const std::wstring& tagName, const stdPath& tagDirectory);
size_t getTagListIndexByName(std::wstring name);
extern fastList<tag*> tagList;