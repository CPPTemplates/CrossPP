#pragma once
#include "attachmentDirectionData.h"
struct buttonData : attachmentDirectionData
{
	int ticksToPress = 0;
	buttonData(const directionID& attachmentDirection = directionID::negativeY, cint& ticksPressed = 0) : attachmentDirectionData(attachmentDirection), ticksToPress(ticksPressed) {}
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeValue(nbtSerializer& s) override;
};