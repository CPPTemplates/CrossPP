#pragma once
#include <vector>
#include "optimization/debug.h"
#include <stdexcept>

#include "interface/idestructable.h"
#include <filesystem>
#include <initializer_list>
#include <utility>
#include "globalFunctions.h"

// super fast solution for iterating through a list while deleting certain items and adding items
constexpr bool keepindicator = true;
constexpr bool eraseindicator = !keepindicator;
template <typename t>
struct fastList : IDestructable
{
	t *baseArray = nullptr;
	size_t size = 0;
	bool *mask = nullptr;
	size_t newSize = 0;
	std::vector<t> addList = std::vector<t>();

	inline explicit fastList(const size_t &size = 0, cbool &initializeToDefault = true)
		: baseArray(initializeToDefault ? new t[size]() : new t[size]),
		  size(size),
		  mask(nullptr),
		  newSize(size),
		  addList(std::vector<t>())
	{
	}

	inline fastList(const std::initializer_list<t> &values) : baseArray(new t[values.size()]),
															  size(values.size()),
															  mask(nullptr),
															  newSize(values.size()),
															  addList(std::vector<t>())
	{
		std::copy(values.begin(), values.end(), baseArray);
	}

	inline fastList(const fastList &other) : baseArray(new t[other.size]), size(other.size),
											 mask(other.mask ? new bool[other.size] : nullptr),
											 newSize(other.newSize),
											 addList(other.addList)
	{
		if (other.mask)
		{
			std::copy(other.mask, other.mask + other.size, mask);
		}

		std::copy(other.baseArray, other.baseArray + other.size, baseArray);
	}

	void swap(fastList<t> &with)
	{
		std::swap(addList, with.addList);
		std::swap(baseArray, with.baseArray);
		std::swap(mask, with.mask);
		std::swap(newSize, with.newSize);
		std::swap(size, with.size);
	}

	// inline fastList& operator=(fastList copy) = default;
	inline fastList &operator=(fastList copy)
	{
		this->swap(copy);
		return *this;
	}

	// for iterative functions
	inline t *begin() const
	{
		return baseArray;
	}
	inline t *end() const
	{
		return baseArray + size;
	}

	inline void removeDoubles()
	{
		update();
		std::vector<t> newArray = std::vector<t>();
		// remove all double elements from the list
		for (size_t i = 0; i < size; i++)
		{
			const t current = baseArray[i];
			if (std::find(newArray.begin(), newArray.end(), current) == newArray.end())
			{
				newArray.push_back(current);
			}
		}
		size = newArray.size();
		delete[] baseArray;
		baseArray = new t[size];
		std::copy(newArray.begin(), newArray.end(), baseArray);
	}

	// refresh the list (add and remove selected items)
	inline void update()
	{
		if (mask || size != newSize)
		{
			// needs to update
			// delete according to the mask: false = do not delete, true = delete
			t *newArray = new t[newSize];
			size_t newIndex = 0;
			// iterate through old array
			for (size_t oldindex = 0; oldindex < size; oldindex++)
			{
				if ((!mask) || mask[oldindex])
				{
					newArray[newIndex] = baseArray[oldindex];
					++newIndex;
				} // else skip
			}
			if (addList.size())
			{
				// add the 'addlist' array
				std::copy(addList.begin(), addList.end(), &newArray[newIndex]);
			}
			// update variables
			size = newSize;
			if (mask)
			{
				delete[] mask;
				mask = nullptr;
			}
			addList.clear();
			delete[] baseArray;
			baseArray = newArray;
		}
	}
	inline void generateMask()
	{
		mask = new bool[size];
		std::fill(mask, mask + size, keepindicator);
	}
	inline void erase(const t &element)
	{
		csize_t &index = find(element);
		if (index != std::wstring::npos)
		{
			erase(index);
		}
	}
	inline void erase(csize_t &index)
	{
		if (!mask)
		{
			generateMask();
		}
		if (mask[index] != eraseindicator)
		{
			newSize--;
			mask[index] = eraseindicator;
		}
	}

	inline void erase(const t *const &pointer)
	{
		erase(pointer - baseArray);
	}

	// find the index of an element with a comparison function
	template <class compareFunction>
	inline size_t findFunction(const compareFunction &&function) const
	{
		for (size_t index = 0; index < size; index++)
		{
			if (function(baseArray[index]))
				return index;
		}
		return -1;
	}

	// find the index of an element
	inline size_t find(const t &element) const
	{
		for (size_t index = 0; index < size; index++)
		{
			if (baseArray[index] == element)
				return index;
		}
		return (size_t)-1;
	}

	// this is slow
	inline void insertAndUpdate(const t &element, size_t index)
	{
		// mask is null, that will continue to be so
		update();

		newSize = size + 1;
		t *newarray = new t[newSize];
		// 0 to index: old 0 to index
		std::copy(baseArray, baseArray + index, newarray);
		// index: new value
		newarray[index] = element;
		// index + 1 to size + 1: old index to size
		std::copy(baseArray + index, baseArray + size, newarray + index + 1);
		delete[] baseArray;
		baseArray = newarray;
		size = newSize;
	}
	inline void push_back(const t &element)
	{
		addList.push_back(element);
		newSize++;
	}
	inline void push_back(t *const begin, t *const end)
	{
		addList->insert(addList.end(), begin, end);
		newSize += end - begin;
	}
	inline void push_back(std::initializer_list<t> elements)
	{
		addList.insert(addList.end(), elements.begin(), elements.end());
		newSize += elements.size();
	}
	template <typename indexType>
	inline t &operator[](const indexType &index) const
	{
		if constexpr (isDebugging)
		{
			// index can't be less than 0
			if ((size_t)index >= size)
			{
				throw std::out_of_range("index out of range");
			}
		}

		return baseArray[index];
	}

	inline void clear()
	{
		if (mask)
			delete[] mask;
		mask = nullptr;
		delete[] baseArray;
		baseArray = new t[0];
		size = 0;
		newSize = 0;
		addList.clear();
	}
	/*
	//update the list before doing this
	inline void deleteMembers() override
	{
		for (int i = 0; i < size; i++)
		{
			delete basearray[i];
		}
	}
	*/
	inline ~fastList() override
	{
		// dont delete the elements!
		delete[] baseArray;
		baseArray = nullptr;
		if (mask)
		{
			delete[] mask;
			mask = nullptr;
		}
	}
};

// WILL NOT SERIALIZE LIST MEMBERS
template <typename streamType, typename t>
inline void serializeList(streamType &stream, cbool &write, fastList<t> *&list)
{
	if (write)
	{
		serialize(stream, write, list->size);
	}
	else
	{
		size_t size = 0;
		serialize(stream, write, size);
		list = new fastList<t>(size, false);
	}
};
