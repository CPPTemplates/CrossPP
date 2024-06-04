#include "SFML/Network.hpp"
#include <SFML/Network/Packet.hpp>
#include "gzipIncluder.h"
#pragma once
struct sf::Packet : public sf::Packet
{
private:
    //std::string sf::Packet;//this will keep the c_str pointer alive as long as it's needed
    //virtual const void* onSend(std::size_t& size) override
    //{
    //    const void* srcData = getData();
    //    std::size_t srcSize = getDataSize();
    //    sf::Packet = gzip::compress((const char*)srcData, srcSize, 1);
    //    size = sf::Packet.size();
    //    return sf::Packet.data();
    //
    //}
    //virtual void onReceive(const void* data, std::size_t size) override
    //{
    //    std::string result = gzip::decompress((const char*)data, size);
    //    append(result.data(), result.size());
    //}
};