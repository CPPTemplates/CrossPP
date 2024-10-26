#pragma once
#include "taskAI.h"
constexpr fp standardDetectionRange = 0x10;
struct targetAI : taskAI
{
	uuid target = uuid();
	fp proximity = 0;

	virtual bool shouldExecute() override;
	bool targetNear(const entity* targetEntity, fp proximity = 0) const;
	bool targetNear() const;
	entity* findTarget() const;
	targetAI(entity* connectedEntity, cfp& proximity = standardDetectionRange) :taskAI(connectedEntity), proximity(proximity) {}
	virtual void updateTask() override;
	virtual void serializeValue(nbtSerializer& s) override;
};