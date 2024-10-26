#include "stairsData.h"
void stairsData::serializeValue(nbtSerializer& s)
{
	facingData::serializeValue(s);
	upsideDownData::serializeValue(s);
}