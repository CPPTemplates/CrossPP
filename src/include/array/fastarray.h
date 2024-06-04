#include "interface/idestructable.h"
#include "globalFunctions.h"
#include <ranges>
#include "optimization/debug.h"
#include <stdexcept>
#include "optimization/macrotricks.h"
#pragma once
//this array does everything with pointers.
//the only difference is that at debug mode it checks if the index is in range.
template<typename t>
struct fastArray :IDestructable
{
	t* baseArray = nullptr;
	fsize_t size = 0;
	//template<typename t>
	inline fastArray(const std::initializer_list<t>& list) : baseArray(new t[list.size()]), size((fsize_t)list.size())
	{
		std::copy(list.begin(), list.end(), begin());
	}
	inline fastArray(t* const& baseArray, cfsize_t& size) : baseArray(baseArray), size(size)
	{

	}
	inline fastArray(cfsize_t& size = 0, cbool& initializeToDefault = true) : baseArray(initializeToDefault ? new t[size]() : new t[size]), size(size)
	{

	}

	inline fastArray(const fastArray& other) :
		baseArray(new t[other.size]),
		size(other.size)
	{
		std::copy(other.begin(), other.end(), begin());
	}

	void swap(fastArray& with)
	{
		std::swap(baseArray, with.baseArray);
		std::swap(size, with.size);
	}

	inline fastArray& operator=(fastArray copy)
	{
		this->swap(copy);
		return *this;
	}

	template<typename offsetT>
	inline t& operator[](const offsetT& index) const
	{
		t* indexPtr = baseArray + index;
		return getPointerValue(indexPtr);
	}

#define newMacro(type, copySize) fastArray<type> result = fastArray<type>((copySize).size);
	addOperators(newMacro, fastArray, fastArray<t2>, inline)
#undef newMacro

		inline t& getPointerValue(t* const& ptr) const
	{
		checkPointerIfDebugging(ptr);
		return *ptr;
	}
	inline void checkPointerIfDebugging(t* const& ptr) const
	{
		if constexpr (isDebugging)
		{
			checkPointer(ptr);
		}
	}
	inline void checkPointer(t* const& ptr) const
	{
		if (ptr < baseArray || ptr >= baseArray + size)
		{
			throw std::out_of_range("index out of range");
		}
	}
	inline ~fastArray() override
	{
		delete[] baseArray;
	}
	inline t* begin() const
	{
		return baseArray;
	}
	inline t* end() const
	{
		return baseArray + size;
	}
};