#pragma once
#include "blockData.h"
struct fallingBlockData : virtual blockData
{
	fallingBlockData() :blockData() {}
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
};