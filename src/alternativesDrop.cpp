#include "alternativesDrop.h"
inline std::vector<itemStack> alternativesDrop::roll(const dropData& data, std::mt19937& randomToUse) const
{
	std::vector<itemStack> result = std::vector<itemStack>();
	for (lootTable* alternative : alternatives)
	{
		result = alternative->roll(data, randomToUse);
		if (result.size())
		{
			return result;
		}
	}
	return std::vector<itemStack>();
}

alternativesDrop::~alternativesDrop()
{
	for (lootTable* const& alternative : alternatives)
	{
		delete alternative;
	}
}