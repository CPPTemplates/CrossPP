#pragma once
#include "targetAI.h"
constexpr int maxTicksAngry = 0x100;
struct angerAI : targetAI
{
	int ticksAngry = 0;
	virtual bool shouldExecute() override;
	virtual bool continueExecuting() override;
	angerAI(entity* const& connectedEntity) :targetAI(connectedEntity) {}
	virtual void serializeValue(nbtSerializer& s) override;
};