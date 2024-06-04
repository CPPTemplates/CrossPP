#include "chestData.h"
#include "rectangularSlotContainer.h"
#include "lootTable.h"
#include "dropData.h"
#include "nbt/nbtSerializer.h"
#include "math/random/random.h"
chestData::chestData()
{
	slots = new rectangularSlotContainer(cveci2(StandardInventoryColumnCount, StandardInventoryRowCount));
}

void chestData::onBlockRemove(tickableBlockContainer* containerIn, cveci2& position)
{
	//convert slots to floating slots
	cvec2 floatingSlotPosition = (cvec2)position + cvec2(0.5, 0);
	generateChestLoot();
	slots->dropContent(containerIn, floatingSlotPosition);
}

chestData::~chestData()
{

	delete slots;
}

void chestData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	((nbtSerializable*)slots)->serialize(s, std::wstring(L"chest slots"));

	if ((s.write ? (lootTableName != std::wstring()) : true) && s.serializeValue(std::wstring(L"loot table name"), lootTableName))
	{
		s.serializeValue(std::wstring(L"loot table seed"), lootTableSeed);
	}
}

void chestData::generateChestLoot()
{
	if (lootTableName != std::wstring())
	{
		auto it = chestLootTables.find(lootTableName);
		if (it != chestLootTables.end())
		{
			std::mt19937 randomToUse = getRandomFromSeed(lootTableSeed ? (ull)lootTableSeed : rand<ull>(currentRandom));
			const std::vector<itemStack> chestLoot = it->second->roll(dropData(), randomToUse);
			std::vector<int> chestPositions = std::vector<int>();
			cint slotCount = slots->rowsAndColumns.x * slots->rowsAndColumns.y;
			for (int i = 0; i < slotCount; i++)
			{
				chestPositions.push_back(i);
			}
			std::shuffle(chestPositions.begin(), chestPositions.end(), randomToUse);

			for (size_t stackIndex = 0; stackIndex < chestLoot.size(); stackIndex++)
			{
				itemStack& slotToFill = slots->slots[chestPositions[stackIndex]];

				slotToFill = chestLoot[stackIndex];//dont copy() the stack, the original is not needed
			}
		}
		//reset data
		lootTableName = std::wstring();
		lootTableSeed = 0;
	}
}