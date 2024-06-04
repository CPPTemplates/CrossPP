#pragma once
#include "blockData.h"
struct openData : virtual blockData
{
	bool isOpen = false;
	openData(cbool& isOpen = false) :isOpen(isOpen) {}
	virtual void serializeValue(nbtSerializer& s) override;
};