#include "avoidEntityAI.h"
#include "idConverter.h"
void avoidEntityAI::serializeValue(nbtSerializer& s)
{
	idConverter::serializeID<entityID>(s, std::wstring(L"entity to avoid"), entityTypeToAvoid, &s.converter->entityIDConverter);
}