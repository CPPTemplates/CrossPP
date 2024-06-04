#pragma once
#include "attachmentDirectionData.h"
struct leverData : attachmentDirectionData
{
	bool active = false;
	leverData(const directionID& attachmentDirection = directionID::negativeY, cbool& active = false) : attachmentDirectionData(attachmentDirection), active(active) {}
};