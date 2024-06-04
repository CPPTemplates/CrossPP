#pragma once
#include "nbt/nbtData.h"
#include <filesystem>
#include <string>
#include <vector>
#include "filesystem/serializer.h"
#include "globalFunctions.h"
#include "nbt/nbtDataTag.h"
struct nbtCompound :nbtData
{
	std::vector<nbtData*> children;
	virtual bool serialize(const streamSerializer& s) override;
	//bool serialize(streamSerializer& s);
	static bool serialize(nbtCompound*& compound, cbool& write, const stdPath& path);
	static bool serialize(nbtCompound*& compound, streamSerializer& s);
	static void serializeChild(const streamSerializer& s, nbtData*& child, const nbtDataTag& childTag);
	static nbtData* createNBTData(const std::wstring& name, const nbtDataTag& tag);
	nbtCompound(const std::wstring& name, const nbtDataTag& dataTag = nbtDataTag::tagCompound) :nbtData(name, dataTag), children(std::vector<nbtData*>()) {}
	virtual ~nbtCompound() override;
	virtual bool serialize(cbool& write, std::wstring& s) override;
	static nbtDataTag getTag(const std::wstring& value);
	static nbtCompound fromString(std::wstring stringifiedNBT);
	virtual bool compare(const nbtData& other) const override;

};