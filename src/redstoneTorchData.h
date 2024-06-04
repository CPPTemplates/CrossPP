#pragma once
#include "attachmentDirectionData.h"
struct redstoneTorchData : attachmentDirectionData
{
	powerLevel lastAttachedBlockPowerLevel = 0;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
};