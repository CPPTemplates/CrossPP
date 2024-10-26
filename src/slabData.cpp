#include "slabData.h"
#include "nbt/nbtSerializer.h"
void slabData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"slab type"), type);
}