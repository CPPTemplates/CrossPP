#pragma once
#include "array/fastlist.h"
#include "array/arrayFunctions/arrayFunctions.h"
std::wstring removeNameSpace(const std::wstring& name);
template<typename dataType, typename idType>
struct idList : public fastList<dataType>
{
	inline dataType& operator[](const idType& index) const
	{
		return fastList<dataType>::operator[]((int)index);
	}
	using fastList<dataType>::operator[];
	//inline dataType& operator[](cint& index) const
	//{
	//	return fastList<dataType>::operator[](index);
	//}
	idType getIDByName(std::wstring name) const;
	idList() {}
	idList(const fastList<dataType>& list) :fastList<dataType>(list) {}
	virtual ~idList() override {
		deleteAllElements(*this);
	}
	//do not allow a copy constructor
	inline idList& operator=(idList&& copy)
	{
		this->swap(copy);
		return *this;
	}
};

template<typename dataType, typename idType>
inline idType idList<dataType, idType>::getIDByName(std::wstring name) const
{
	name = removeNameSpace(name);
	return (idType)fastList<dataType>::findFunction([name](const auto& a) {return a->name == name; });

}