#include "buttonData.h"
#include "tickableBlockContainer.h"
#include "nbt/nbtSerializer.h"
bool buttonData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (!attached(containerIn, position)) {
		return attachedBlockData::tick(containerIn, position);
	}
	if (ticksToPress > 0)
	{
		ticksToPress--;
		if (ticksToPress > 0)
		{
			return true;
		}
		else
		{
			containerIn->addUpdatePosition(position);
		}
	}
	return false;
}

void buttonData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	s.serializeValue(std::wstring(L"ticks to press"), ticksToPress);
	attachmentDirectionData::serializeValue(s);
}