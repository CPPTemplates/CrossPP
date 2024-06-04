#pragma once
#include "blockData.h"
struct ageData : virtual blockData
{
	int age = 0;
	ageData(cint& age = 0) :age(age) {}
	virtual void serializeValue(nbtSerializer& s) override;
};