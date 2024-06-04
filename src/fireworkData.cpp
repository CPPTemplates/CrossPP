#include "fireworkData.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeList.h"
void fireWorkData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"flight duration"), flightDuration);
	serializeListOfSerializables(s, std::wstring(L"firework stars"), fireworkStars);
}