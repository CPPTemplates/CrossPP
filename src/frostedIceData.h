#pragma once
#include "ageData.h"
struct frostedIceData : ageData
{
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
};