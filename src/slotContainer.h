#pragma once
#include "nbt/nbtSerializable.h"
#include "itemStack.h"
struct slotContainer :public nbtSerializable
{
	//add a itemStack to this inventory, for example 32 coal.
	//it edits the value of the itemStack. (if 14 fits, the value becomes 18)
	virtual bool addStack(itemStack& s);
	virtual bool substractStack(itemStack& stack);
	virtual void serializeValue(nbtSerializer& s) override;
	virtual void clearData();
};