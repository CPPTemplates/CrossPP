#include "trapDoorData.h"
void trapDoorData::serializeValue(nbtSerializer& s)
{
	facingData::serializeValue(s);
	upsideDownData::serializeValue(s);
	openData::serializeValue(s);
}