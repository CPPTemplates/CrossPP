#include "nbt/nbtSerializer.h"
#include "bambooData.h"
void bambooData::serializeValue(nbtSerializer& s)
{
	ageData::serializeValue(s);
	s.serializeValue(std::wstring(L"leaves size"), leavesSize);
	s.serializeValue(std::wstring(L"stalk thickness"), stalkThickness);
}