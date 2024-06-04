#pragma once
#include "doubleBlockData.h"
#include "attachedBlockData.h"
struct doubleAttachedBlockData : doubleBlockData, attachedBlockData
{
	doubleAttachedBlockData(cbool& isPart0 = false) : doubleBlockData(isPart0), attachedBlockData() {}
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeValue(nbtSerializer& s) override
	{
		return doubleBlockData::serializeValue(s);
	}

};