#include "attachmentDirectionData.h"
#include "nbt/nbtSerializer.h"
directionID attachmentDirectionData::getAttachmentDirection(tickableBlockContainer * /*containerIn*/, cveci2 & /*position*/) const
{
	return attachmentDirection;
}

void attachmentDirectionData::serializeValue(nbtSerializer &s)
{
	s.serializeValue(std::wstring(L"attachment direction"), attachmentDirection);
}