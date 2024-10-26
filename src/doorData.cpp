#include "doorData.h"
void doorData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	doubleBlockData::serializeValue(s);
	facingData::serializeValue(s);
	openData::serializeValue(s);
}