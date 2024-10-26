#pragma once
#include "blockData.h"
struct upsideDownData : virtual blockData
{
	bool upsideDown = false;
	upsideDownData(cbool& upsideDown = false) :upsideDown(upsideDown) {}
	virtual void serializeValue(nbtSerializer& s) override;
};