#include <SFML/System/InputStream.hpp>
#include <memory>
#include <fstream>
#pragma once

inline sf::Int64 getStreamSize(std::istream& stream){
    
        const std::streampos& curr = stream.tellg();

        stream.seekg(0, stream.end);
        const std::streampos& pos1 = stream.tellg();

        stream.seekg(curr, stream.beg);

        return pos1;
}

// https://stackoverflow.com/questions/28312737/c-implementing-sfml-inputstream
class sfmlInputStream final : public sf::InputStream
{
    std::shared_ptr<std::istream> m_source;
    const sf::Int64 size;

public:
    explicit sfmlInputStream(std::shared_ptr<std::istream> stream) : m_source(stream), size(getStreamSize(*stream)) {}

    sf::Int64 read(void *data, sf::Int64 size) final
    {
        if(!m_source->good()){
            return -1;
        }
        const auto maximumReadableSize = this->size - m_source->tellg();
        const sf::Int64& max = math::minimum(size, maximumReadableSize);
        m_source->read(static_cast<char *>(data), max);

        // return m_source->gcount();
        return m_source->good() ? max : -1;
    }
    sf::Int64 seek(sf::Int64 position) final
    {
        const sf::Int64& max = math::minimum(position, this->size);
        m_source->seekg(max, m_source->beg);

        return m_source->good() ? max : -1;
        // return m_source->gcount();
        // return m_source->good()? m_source->gcount() : -1;
    }
    sf::Int64 tell() final
    {
        std::streampos pos = m_source->tellg();
        return pos;
    }
    sf::Int64 getSize() final
    {
        return size;
    }
};