#include "jigsawData.h"
#include "nbt/nbtSerializer.h"
#include "idConverter.h"
void jigsawData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	facingData::serializeValue(s);
	s.serializeValue(std::wstring(L"jigsaw name"), jigsawName);
	s.serializeValue(std::wstring(L"target name"), targetName);
	s.serializeValue(std::wstring(L"target pool name"), targetPoolName);
	s.serializeValue(std::wstring(L"rollable"), rollable);
	s.serializeValue(std::wstring(L"keep jigsaws"), keepJigsaws);
	s.serializeValue(std::wstring(L"levels"), (int&)levels);
	idConverter::serializeID(s, std::wstring(L"turns into"), (itemID&)turnsInto, s.converter ? &s.converter->itemIDConverter : nullptr);
}
