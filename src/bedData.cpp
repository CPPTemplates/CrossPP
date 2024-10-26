#include "bedData.h"
void bedData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	doubleBlockData::serializeValue(s);
	facingData::serializeValue(s);
}