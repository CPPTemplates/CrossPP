#include "statusEffect.h"
#include "idConverter.h"
void statusEffect::serializeValue(nbtSerializer& s)
{
	idConverter::serializeID(s, std::wstring(L"id"), identifier, s.converter ? &s.converter->statusEffectIDConverter : nullptr);
	s.serializeValue(std::wstring(L"duration"), ticksDuration);
	s.serializeValue(std::wstring(L"potency"), (int&)potency);
}
