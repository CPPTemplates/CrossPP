#pragma once
#include "type/conversion.h"
template<typename t>
struct nbtDataArray :nbtData
{
	size_t arraySize = 0;
	t* data = nullptr;
	virtual inline bool serialize(const streamSerializer& s) override
	{
		int intSize = (int)arraySize;
		s.serialize(intSize);

		if (!s.write)
		{
			arraySize = intSize;
			data = new t[arraySize];
		}
		s.serialize(data, arraySize);
		return true;
	}
	virtual inline bool serialize(cbool& write, std::wstring& s) override
	{
		if (write)
		{
			for (size_t i = 0; i < arraySize; i++)
			{
				s += std::to_wstring(data[i]);
				if (i < (arraySize - 1))
				{
					s += std::wstring(L",");
				}
			}
		}
		else
		{
			const wstringContainer& container = split_string(s, std::wstring(L","));
			arraySize = container.size();
			//size of container will always be 1 or more (when nothing is in the string, it will return 1 string containing "")
			assumeInRelease1(arraySize);
			data = new t[arraySize];
			for (size_t i = 0; i < arraySize; i++)
			{
				convertTo<t>(s, data[i]);
			}
		}
		return true;
	}
	nbtDataArray(const std::wstring& name, const nbtDataTag& dataTag) :nbtData(name, dataTag), arraySize(0), data(nullptr) {}
	inline virtual ~nbtDataArray() override
	{
		delete data;
	}
	inline bool serializeValue(cbool& write, void* value, int& size)
	{
		if (write)
		{
			//copy array
			arraySize = size;
			data = new t[size];
			std::copy(value, (t*)value + size, data);
		}
		else
		{
			size = arraySize;
			value = new t[size];
			//copy array
			std::copy(data, data + arraySize, value);
		}
		return true;
	}
	virtual bool compare(const nbtData& other) const override;
};
template<typename t>
inline bool nbtDataArray<t>::compare(const nbtData& other) const
{
	return (((const nbtDataArray<t>&)other).arraySize == arraySize) &&
		(std::equal(data, data + arraySize, ((const nbtDataArray<t>&)other).data));
}