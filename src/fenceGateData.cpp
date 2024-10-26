#include "fenceGateData.h"
void fenceGateData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	facingData::serializeValue(s);
	openData::serializeValue(s);
}