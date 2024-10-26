#pragma once
#include "filesystem/filemanager.h"
#include "globalFunctions.h"
struct nbtSerializer;
struct nbtSerializable
{
	//ONLY CALL THIS FUNCTION IF YOU ARE SURE THERE ARE NOT TWO OF THOSE IN YOUR COMPOUND!
	virtual void serializeValue(nbtSerializer& s);
	void serialize(nbtSerializer& s, const std::wstring& name = std::wstring());
	void clone(nbtSerializable& cloneTo);
	bool compare(nbtSerializable& other);
	bool serialize(const std::wstring& name, const stdPath& path, cbool& write);
	std::wstring toString();

	template<typename t>
	t clone();

};
template<typename t>
inline t nbtSerializable::clone()
{
	t clonedObject = t();
	clone(clonedObject);
	return clonedObject;
}