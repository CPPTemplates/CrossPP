#pragma once
#include "blockData.h"
struct doubleBlockData : virtual blockData
{
	bool isPart0 = false;
	doubleBlockData(cbool& isPart0 = false) : isPart0(isPart0) {}
	//dont remove the other block in 'onblockremove', you will get stuck in an infinite loop
	virtual void serializeValue(nbtSerializer& s) override;
};
veci2 getOtherPartRelativeLocation(const blockID& id, cbool& isPart0, const directionID& directionFacing);