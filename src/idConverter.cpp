#include "idConverter.h"
#include "folderList.h"
#include "entityData.h"
#include "itemData.h"
#include "biomeData.h"
#include "enchantmentData.h"
#include "statusEffectData.h"
#include "gameModeData.h"
#include "fireworkShapeData.h"

std::vector<idConverter*> converterList = std::vector<idConverter*>();

idConverter::idConverter(cint& oldVersion, cint& newVersion)
{
	this->oldVersion = oldVersion;
	this->newVersion = newVersion;
	nbtCompound* compound;
	cbool& write = false;
	if (nbtCompound::serialize(compound, write, fileVersionsFolder / (std::to_wstring(oldVersion) + nbtFileExtension)))
	{
		nbtSerializer s = nbtSerializer(*compound, write, false);
		itemIDConverter = readIDList<itemData, itemID>(s, itemList, std::wstring(L"item id"));
		entityIDConverter = readIDList<entityData, entityID>(s, entityDataList, std::wstring(L"entity id"));
		biomeIDConverter = readIDList<biomeData, biomeID>(s, biomeDataList, std::wstring(L"biome id"));
		enchantmentIDConverter = readIDList<enchantmentData, enchantmentID>(s, enchantmentDataList, std::wstring(L"enchantment id"));
		statusEffectIDConverter = readIDList<statusEffectData, statusEffectID>(s, statusEffectDataList, std::wstring(L"status effect id"));
		gameModeIDConverter = readIDList<gameModeData, gameModeID>(s, gameModeDataList, std::wstring(L"game mode id"));
		fireworkShapeIDConverter = readIDList<fireworkShapeData, fireworkShapeID>(s, fireworkShapeDataList, std::wstring(L"firework shape id"));
		delete compound;
	}
}

void idConverter::writeIDsToFile()
{
	const stdPath path = fileVersionsFolder / (std::to_wstring(currentFileVersionID) + nbtFileExtension);
	//get last converter
	if (stdFileSystem::exists(path))
	{
		//check if nothing has changed
		idConverter* converter = new idConverter(currentFileVersionID);
		if (
			findArrayDifference(itemList, converter->itemIDConverter) ||
			findArrayDifference(entityDataList, converter->entityIDConverter) ||
			findArrayDifference(biomeDataList, converter->biomeIDConverter) ||
			findArrayDifference(enchantmentDataList, converter->enchantmentIDConverter) ||
			findArrayDifference(statusEffectDataList, converter->statusEffectIDConverter) ||
			findArrayDifference(gameModeDataList, converter->gameModeIDConverter) ||
			findArrayDifference(fireworkShapeDataList, converter->fireworkShapeIDConverter)
			)
		{
			handleError(std::wstring(L"increment the file version"));
		}
	}
	else
	{
		nbtCompound* compound = new nbtCompound(std::wstring(L"idconverter"));
		cbool& write = true;
		nbtSerializer s = nbtSerializer(*compound, write);
		//output file stream
		writeIDList(s, itemList, std::wstring(L"item id"));
		writeIDList(s, entityDataList, std::wstring(L"entity id"));
		writeIDList(s, biomeDataList, std::wstring(L"biome id"));
		writeIDList(s, enchantmentDataList, std::wstring(L"enchantment id"));
		writeIDList(s, statusEffectDataList, std::wstring(L"status effect id"));
		writeIDList(s, gameModeDataList, std::wstring(L"game mode id"));
		writeIDList(s, fireworkShapeDataList, std::wstring(L"firework shape id"));

		//we use a pointer because technically it could modify the pointer
		nbtCompound::serialize(compound, write, path);

		delete compound;
	}
}
idConverter* getConverter(cint& oldVersion)
{
	if (oldVersion == currentFileVersionID)
	{
		return nullptr;
	}
	for (idConverter* const& converter : converterList)
	{
		if (converter->oldVersion == oldVersion)
		{
			return converter;
		}
	}
	idConverter* const& converter = new idConverter(oldVersion, currentFileVersionID);
	converterList.push_back(converter);
	return converter;
}