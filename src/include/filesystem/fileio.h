#pragma once
#include "globalFunctions.h"
#include "filemanager.h"
#include <fstream>
#include <string>
#include <iostream>

constexpr std::ios_base::openmode getOpenMode(cbool& write)
{
	return (write ? std::ios::out : std::ios::in) | std::ios::binary;
}

//don't use or delete buffer, it will destroy itself once it goes out of scope
inline std::fstream getAcceleratedFstream(const stdPath& path, cbool& write, std::shared_ptr<char[]>& buffer, cint& bufferSize)
{
	std::fstream stream;
	stream.open(path.c_str(), getOpenMode(write));
	if (stream.good())
	{
		//give a buffer to read faster
		//https://stackoverflow.com/questions/11563963/how-to-write-a-large-buffer-into-a-binary-file-in-c-fast/39097696#39097696
		buffer = std::shared_ptr<char[]>(new char[bufferSize]);
		stream.rdbuf()->pubsetbuf(buffer.get(), bufferSize);
	}
	return stream;
}



template<typename streamType, typename valueType>
inline void writeToStream(streamType& stream, const valueType& value)
{
	static_assert(!std::is_pointer<valueType>::value, "you can't just write a pointer to a stream!");
	stream.write(castout(&value), sizeof(valueType));
}
template<typename streamType, typename valueType>
inline void readFromStream(streamType& stream, valueType& value)
{
	static_assert(!std::is_pointer<valueType>::value, "you can't just write a pointer to a stream!");
	static_assert(!std::is_const<valueType>::value, "you can't just read to a const value!");
	stream.read(castin(&value), sizeof(valueType));
}

template<typename streamType, typename valueType>
inline void writeToStream(streamType& stream, valueType* const& valuesPointer, cint& arraySize)
{
	static_assert(!std::is_pointer<valueType>::value, "you can't just write a pointer to a stream!");
	stream.write(castout(valuesPointer), sizeof(valueType) * arraySize);
}

template<typename streamType, typename valueType>
inline void readFromStream(streamType& stream, valueType* const& valuesPointer, cint& arraySize)
{
	static_assert(!std::is_pointer<valueType>::value, "you can't just write a pointer to a stream!");
	static_assert(!std::is_const<valueType>::value, "you can't just read to a const value!");
	stream.read(castin(valuesPointer), sizeof(valueType) * arraySize);
}