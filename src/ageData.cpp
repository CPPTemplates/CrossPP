#include "ageData.h"
#include "nbt/nbtSerializer.h"
void ageData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"age"), age);
}