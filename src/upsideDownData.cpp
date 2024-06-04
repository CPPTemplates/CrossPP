#include "upsideDownData.h"
#include "nbt/nbtSerializer.h"
void upsideDownData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"upside down"), upsideDown);
}