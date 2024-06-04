#include "facingData.h"
#include "nbt/nbtSerializer.h"
void facingData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"facing"), directionFacing);
}