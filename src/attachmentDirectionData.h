#pragma once
#include "attachedBlockData.h"
struct attachmentDirectionData : attachedBlockData
{
	directionID attachmentDirection = directionID();
	virtual directionID getAttachmentDirection(tickableBlockContainer* containerIn, cveci2& position) const override;
	attachmentDirectionData(const directionID& attachmentDirection = directionID::negativeY) :attachmentDirection(attachmentDirection) {}
	virtual void serializeValue(nbtSerializer& s) override;
};