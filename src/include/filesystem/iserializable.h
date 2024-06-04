#pragma once
#include "serializer.h"
#include "file/endian.h"
#include "filesystem/filemanager.h"
struct ISerializable
{
	virtual bool serialize(const streamSerializer& s) = 0;
	bool serialize(const stdPath& path, cbool& write, const std::endian& fileEndianness = std::endian::native);
	void clone(ISerializable& cloneTo);
};