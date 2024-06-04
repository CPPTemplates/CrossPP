#include "blockStateCondition.h"
#include "dropData.h"
#include "nbt/nbtSerializer.h"
bool blockStateCondition::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	if (data.harvestedBlockData)
	{
		nbtCompound checkCompound = nbtCompound(std::wstring(L"checkCompound"));
		nbtSerializer s = nbtSerializer(checkCompound, true, false);
		data.harvestedBlockData->serializeValue(s);
		s.write = false;

		for (size_t i = 0; i < blockStateNames.size(); i++)
		{
			int val = 0;
			if (s.serializeValue(blockStateNames[i], val))
			{
				int checkValue;
				if (convertToInt(blockStateValues[i], checkValue))
				{
					if (checkValue != val)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				//can't find block state property, so just return true
				return true;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}