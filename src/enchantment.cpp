#include "enchantment.h"
#include "idConverter.h"
#include "enchantmentData.h"
#include "nbt/nbtSerializer.h"
void enchantment::serializeValue(nbtSerializer& s)
{
	idConverter::serializeID(s, std::wstring(L"id"), identifier, s.converter ? &s.converter->enchantmentIDConverter : nullptr);
	s.serializeValue(std::wstring(L"power"), power);
}

std::wstring enchantment::toWString() const
{
	std::wstring enchantmentName = enchantmentDataList[(int)identifier]->name;
	enchantmentName = replace(enchantmentName, std::wstring(L"_"), std::wstring(L" "));
	if (power != 1 || power < (int)enchantmentDataList[(int)identifier]->powerRanges.size)
	{
		enchantmentName += L" " + std::to_wstring(power);
	}
	return enchantmentName;
}