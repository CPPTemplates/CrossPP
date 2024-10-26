#pragma once
#include "blockData.h"
struct attachedBlockData : virtual blockData
{
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual bool attached(tickableBlockContainer* containerIn, cveci2& position) const;

	virtual directionID getAttachmentDirection(tickableBlockContainer* containerIn, cveci2& position) const;
};