#include "structureBlockData.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeColor.h"
void structureBlockData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"structure name"), structureName);
	serializeNBTValue(s, std::wstring(L"relative structure position"), relativeStructurePosition);
	serializeNBTValue(s, std::wstring(L"structure size"), structureSize);
	s.serializeValue(std::wstring(L"include entities"), includeEntities);
	s.serializeValue(std::wstring(L"mirror x"), mirrorX);
	s.serializeValue(std::wstring(L"structure rotation"), structureRotation);
	s.serializeValue(std::wstring(L"structure integrity"), structureIntegrity);
	s.serializeValue(std::wstring(L"structure seed"), structureSeed);
	s.serializeValue(std::wstring(L"show bounding box"), showBoundingBox);
	s.serializeValue(std::wstring(L"structure block mode"), mode);
}