#pragma once
#include "angerAI.h"
struct hurtByTargetAI :angerAI
{
	virtual bool shouldExecute() override;
	hurtByTargetAI(entity* const& connectedEntity) :angerAI(connectedEntity) {}
};