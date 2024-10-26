#pragma once
#include "doorData.h"
struct ironDoorData : doorData
{
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
};