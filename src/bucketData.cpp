#include "bucketData.h"
#include "nbt/nbtSerializer.h"
void bucketData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"fluid level"), fillLevel);
}