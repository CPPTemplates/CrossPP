#pragma once
#include "blockData.h"
struct grassBlockData : blockData
{
	virtual void randomTick(tickableBlockContainer* containerIn, cveci2& position) override;
};