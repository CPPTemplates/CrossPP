#include "amountDistribution.h"
#include "constantAmountDistribution.h"
#include "uniformAmountDistribution.h"
#include "binomialAmountDistribution.h"
#include "filesystem/jsonReader.h"
#include "include/optimization/handleError.h"
amountDistribution* parseMinimumAndMaximum(const jsonContainer& container)
{
	csize_t& typeIndex = container.getChildIndex(std::wstring(L"type"));

	if (typeIndex == std::wstring::npos)
	{
		constantAmountDistribution* distribution = new constantAmountDistribution();
		fp fpAmount = 0;
		if (convertToDouble(container.children[0].value, fpAmount))
		{
			distribution->amount = (int)fpAmount;
		}
		else
		{
			handleError(std::wstring(L"invalid amount"));
		}
		return distribution;
	}
	else
	{
		const std::wstring& distributionTypeName = container.children[typeIndex].children[0].value;

		if (distributionTypeName == std::wstring(L"minecraft:uniform"))
		{
			uniformAmountDistribution* distribution = new uniformAmountDistribution();

			fp fpMinimum = 0, fpMaximum = 0;
			if ((!convertToDouble(container.getChild(std::wstring(L"min")).children[0].value, fpMinimum))
				|| (!convertToDouble(container.getChild(std::wstring(L"max")).children[0].value, fpMaximum)))
			{
				handleError(std::wstring(L"could not parse"));
			}
			distribution->minimum = (int)fpMinimum;
			distribution->maximum = (int)fpMaximum;
			return distribution;
		}
		else if (distributionTypeName == std::wstring(L"minecraft:binomial"))
		{
			binomialAmountDistribution* distribution = new binomialAmountDistribution();

			fp fpN = 0;

			if (convertToDouble(container[std::wstring(L"n")].children[0].value, fpN) &&
				convertToDouble(container[std::wstring(L"p")].children[0].value, distribution->p))
			{

			}
			else
			{
				handleError(std::wstring(L"could not parse"));
			}
			return distribution;
		}
		else
		{
			handleError(std::wstring(L"no distribution found with this name"));
		}
	}

	return nullptr;
}