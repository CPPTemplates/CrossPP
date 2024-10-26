#include "durabilityData.h"
#include "nbt/nbtSerializer.h"
void durabilityData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"durability"), durability);
}