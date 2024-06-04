#pragma once
#include "nbt/nbtSerializable.h"
#include "math/uuid.h"
struct playerData : nbtSerializable {
	uuid id;
	std::wstring name;
	virtual void serializeValue(nbtSerializer& s) override;
	bool serialize(cbool& write);
};