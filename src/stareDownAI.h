#pragma once
#include "hurtByTargetAI.h"
struct stareDownAI :hurtByTargetAI
{
	virtual bool shouldExecute() override;
	stareDownAI(entity* const& connectedEntity) :hurtByTargetAI(connectedEntity) {}
};