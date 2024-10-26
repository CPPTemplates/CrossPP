#include "dropCondition.h"
#include "explosionDrop.h"
#include "blockStateCondition.h"
#include "chanceCondition.h"
#include "tableBonusCondition.h"
#include "enchantmentData.h"
#include "alternativeDropCondition.h"
#include "invertedCondition.h"
#include "toolDrop.h"
#include "itemData.h"
dropCondition* readCondition(const jsonContainer& container)
{
	const std::wstring conditionName = container[std::wstring(L"condition")].children[0].value;
	dropCondition* conditionToReturn = nullptr;
	if (conditionName == std::wstring(L"minecraft:survives_explosion"))
	{
		conditionToReturn = new explosionDrop();
	}
	else if (conditionName == std::wstring(L"minecraft:block_state_property"))
	{
		blockStateCondition* condition = new blockStateCondition();

		const jsonContainer& conditionContainer = container[std::wstring(L"properties")];

		for (const jsonContainer& blockStateContainer : conditionContainer.children)
		{
			condition->blockStateNames.push_back(blockStateContainer.value);
			condition->blockStateValues.push_back(blockStateContainer.children[0].value);
		}

		conditionToReturn = condition;
	}
	else if (conditionName == std::wstring(L"minecraft:random_chance"))
	{
		chanceCondition* condition = new chanceCondition();
		convertToDouble(container[std::wstring(L"chance")].children[0].value, condition->chance);
		conditionToReturn = condition;
	}
	else if (conditionName == std::wstring(L"minecraft:table_bonus"))
	{
		tableBonusCondition* bonusCondition = new tableBonusCondition();
		const std::vector<jsonContainer> chanceTableContainers = container[std::wstring(L"chances")].children;
		bonusCondition->chanceTable.resize(chanceTableContainers.size());

		const enchantmentID& requiredEnchantmentID = enchantmentDataList.getIDByName(container[std::wstring(L"enchantment")].children[0].value);

		bonusCondition->enchantmentRequired = requiredEnchantmentID;
		for (size_t i = 0; i < chanceTableContainers.size(); i++)
		{
			convertToDouble(chanceTableContainers[i].value, bonusCondition->chanceTable[i]);
		}
		conditionToReturn = bonusCondition;
	}
	else if (conditionName == std::wstring(L"minecraft:alternative"))
	{
		alternativeDropCondition* currentAlternativeDrop = new alternativeDropCondition();
		const std::vector<jsonContainer> termsContainer = container[std::wstring(L"terms")].children;
		for (const jsonContainer& termContainer : termsContainer)
		{
			currentAlternativeDrop->conditionsToCheck.push_back(readCondition(termContainer));
		}
		conditionToReturn = currentAlternativeDrop;
	}
	else if (conditionName == std::wstring(L"minecraft:inverted"))
	{
		invertedCondition* currentInvertedCondition = new invertedCondition();

		currentInvertedCondition->conditionToInvert = readCondition(container[std::wstring(L"term")]);
		conditionToReturn = currentInvertedCondition;
	}
	else if (conditionName == std::wstring(L"minecraft:match_tool"))
	{
		toolDrop* currentToolDrop = new toolDrop();

		const jsonContainer& predicateContainer = container[std::wstring(L"predicate")];

		csize_t& enchantmentsIndex = predicateContainer.getChildIndex(std::wstring(L"enchantments"));

		if (enchantmentsIndex != std::wstring::npos)
		{
			const std::vector<jsonContainer> enchantmentContainers = predicateContainer.children[enchantmentsIndex].children;
			for (const jsonContainer& enchantmentContainer : enchantmentContainers)
			{
				const std::wstring enchantmentName = enchantmentContainer[std::wstring(L"enchantment")].children[0].value;
				const std::wstring requiredLevelString = enchantmentContainer[std::wstring(L"levels")][std::wstring(L"min")].children[0].value;

				const enchantmentID& enchantmentType = enchantmentDataList.getIDByName(enchantmentName);
				int requiredLevel;
				if (!convertToInt(requiredLevelString, requiredLevel))
				{
					requiredLevel = 1;
				}

				currentToolDrop->requiredEnchantments.push_back(enchantment(enchantmentType, requiredLevel));
			}
		}

		csize_t& itemIndex = predicateContainer.getChildIndex(std::wstring(L"item"));
		if (itemIndex != std::wstring::npos)
		{
			const std::wstring requiredItemName = predicateContainer.children[itemIndex].children[0].value;
			const itemID& requiredItemID = itemList.getIDByName(requiredItemName);
			if ((int)requiredItemID != -1)
			{
				currentToolDrop->requiredTool = requiredItemID;
			}
		}

		conditionToReturn = currentToolDrop;
	}
	if (conditionToReturn)
	{
		return conditionToReturn;
	}
	else
	{
		return new dropCondition();
		//handleError(std::wstring(L"no condition found");
	}
}
bool dropCondition::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	return true;
}