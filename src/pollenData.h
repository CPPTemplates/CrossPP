#pragma once
#include "blockData.h"
struct pollenData : virtual blockData
{
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
};