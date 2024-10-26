#include <filesystem/file/tcpSerializer.h>
#include "filesystem/file/endian.h"
#include "filesystem/serializer.h"
#pragma once
constexpr std::endian defaultEndianness = std::endian::little;
struct nbtSerializer;
struct socketContainer :serializer<tcpSerializer> {
	tcpSerializer s = tcpSerializer();
	socketContainer() : serializer(s, false, defaultEndianness){}
};