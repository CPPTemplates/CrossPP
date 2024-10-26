#pragma once
#include "trapDoorData.h"
struct ironTrapDoorData : trapDoorData
{
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
};