#pragma once
#include "blockData.h"
#include "slabType.h"
struct slabData : blockData
{
	slabType type = slabType::bottomSlab;
	slabData(const slabType& type = slabType::bottomSlab) :type(type) {}
	virtual void serializeValue(nbtSerializer& s) override;

};