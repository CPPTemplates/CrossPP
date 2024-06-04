#include "fireworkStarData.h"
#include "idConverter.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeList.h"
#include "nbt/serializeColor.h"
void fireWorkStarData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"flicker"), flicker);
	s.serializeValue(std::wstring(L"trail"), trail);
	idConverter::serializeID(s, std::wstring(L"shape"), shape, s.converter ? &s.converter->fireworkShapeIDConverter : nullptr);
	serializeNBTValue(s, std::wstring(L"colors"), colors);
	serializeNBTValue(s, std::wstring(L"fade colors"), fadeColors);
}