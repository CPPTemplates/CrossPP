#pragma once
#include <vector>
#include "itemID.h"
#include "biomeID.h"
#include "enchantmentID.h"
#include "statusEffectID.h"
#include "gameModeID.h"
#include "fireworkShapeID.h"
#include "idList.h"
#include "nbt/nbtSerializer.h"
struct idConverter
{
	//oldtonew[oldID] gives newID
	std::vector<itemID> itemIDConverter = std::vector<itemID>();
	std::vector<entityID> entityIDConverter = std::vector<entityID>();
	std::vector<biomeID> biomeIDConverter = std::vector<biomeID>();
	std::vector<enchantmentID> enchantmentIDConverter = std::vector<enchantmentID>();
	std::vector<statusEffectID> statusEffectIDConverter = std::vector<statusEffectID>();
	std::vector<gameModeID> gameModeIDConverter = std::vector<gameModeID>();
	std::vector<fireworkShapeID> fireworkShapeIDConverter = std::vector<fireworkShapeID>();

	int oldVersion = 0;
	int newVersion = currentFileVersionID;

	idConverter(cint& oldVersion, cint& newVersion = currentFileVersionID);

	template<typename dataType, typename idType>
	inline static bool convertID(idType& valueToConvert, const std::vector <dataType>& converter)
	{
		cint index = (int)converter[(int)valueToConvert];
		if (index != -1)
		{
			if constexpr (std::is_same<idType, blockID>::value || std::is_same<idType, itemID>::value)
			{
				if (index >= blockIDCount)
				{
					return false;
				}
			}

			valueToConvert = (idType)index;
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename idType>
	inline static bool serializeID(nbtSerializer& s, const std::wstring& memberName, idType& value, const std::vector<idType>* converter)
	{
		if (s.write)
		{
			return s.serializeValue(memberName, (int&)value);
		}
		else
		{
			if (s.serializeValue(memberName, (int&)value))
			{
				if ((!converter) || convertID<idType>(value, *converter))
				{
					return true;
				}
			}
			return false;
		}
	}

	template<typename dataType, typename idType>
	inline static std::vector<idType> readIDList(nbtSerializer& s, const idList<dataType*, idType>& dataList, const std::wstring& name)
	{
		if (s.push<nbtDataTag::tagList>(name))
		{
			const std::vector<nbtData*> serializedIDList = s.getChildren();
			std::vector<idType> conversionList = std::vector<idType>(serializedIDList.size());
			for (size_t i = 0; i < serializedIDList.size(); i++)
			{
				std::wstring name;
				//*serializedIDList[i]
				s.serializeValue(std::wstring(), name);
				conversionList[i] = dataList.getIDByName(name);
			}
			s.pop();
			return conversionList;
		}
		else
		{
			return std::vector<idType>();
		}
	}
	template<typename dataType, typename idType>
	static void writeIDList(nbtSerializer& s, const idList<dataType*, idType>& dataList, const std::wstring& name)
	{
		if (s.push<nbtDataTag::tagList>(name))
		{
			for (fsize_t i = 0; i < dataList.size; i++)
			{
				s.serializeValue(std::wstring(), dataList[i]->name);
			}
			s.pop();
		}
	}
	static void writeIDsToFile();
	template<typename dataType, typename idType>
	inline static void convertArray(dataType*& idArray, csize_t & idArraySize, const std::vector<idType>& converter)
	{
		//convert everything to the new value
		for (size_t index = 0; index < idArraySize; index++)
		{
			convertID<idType>(idArray[index], converter);
		}
	}
	template<typename dataType, typename idType>
	inline static bool findArrayDifference(const idList<dataType*, idType>& dataList, const std::vector<idType>& converter)
	{
        typedef std::underlying_type_t<idType> ut;
		if (dataList.size != converter.size())
		{
			return true;
		}
		//convert everything to the new value
		for (ut index = 0; index < (ut)converter.size(); index++)
		{
			if ((ut)converter[index] != index)
			{
				return true;
			}
		}
		return false;
	}
};
extern std::vector<idConverter*> converterList;
idConverter* getConverter(cint& oldVersion);