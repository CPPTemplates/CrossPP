#pragma once
#include "dispenserData.h"
struct dropperData :dispenserData
{
	virtual bool dispenseItem(const itemStack& stack, tickableBlockContainer* containerIn, cveci2& position) override;
};