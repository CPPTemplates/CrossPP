#include "filesystem/textfile.h"
#include "filesystem/fileio.h"
#include "nbt/nbtCompound.h"
#include "nbtValue.h"
#include "nbtDataArray.h"
#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <list>
#include <string>
#include "array/wstringFunctions.h"
#include "filesystem/file/endian.h"
#include "filesystem/serializer.h"
#include "globalFunctions.h"
#include "nbt/nbtData.h"
#include "nbt/nbtDataTag.h"
#include <sstream>
#include "optimization/handleError.h"
#include "filesystem/gzipIncluder.h"

bool nbtCompound::serialize(const streamSerializer &s)
{
    switch (dataTag)
    {
    case nbtDataTag::tagList:
    {
        nbtDataTag childTag;
        if (s.write)
        {
            childTag = children.size() ? children[0]->dataTag : nbtDataTag::tagEnd;
        }
        s.serialize(childTag);

        // DONT CHANGE TO SIZE_T
        int childCount;
        if (s.write)
        {
            childCount = (int)children.size();
            s.serialize(childCount);
        }
        else
        {
            s.serialize(childCount);
            children.resize((size_t)childCount);
        }

        for (nbtData *&child : children)
        {
            if (!s.write)
            {
                child = createNBTData(std::wstring(), childTag);
            }
            child->serialize(s);
        }
        break;
    }
    case nbtDataTag::tagCompound:
    {
        if (s.write)
        {
            for (nbtData *&child : children)
            {
                s.serialize(child->dataTag);
                serializeChild(s, child, child->dataTag);
            }
            nbtDataTag endTag = nbtDataTag::tagEnd;
            s.serialize(endTag);
        }
        else
        {
            while (true)
            {
                nbtDataTag currentTag;
                s.serialize(currentTag);
                if (currentTag == nbtDataTag::tagEnd)
                {
                    break;
                }
                else
                {
                    nbtData *childData;
                    serializeChild(s, childData, currentTag);
                    children.push_back(childData);
                }
            }
        }
    }
    break;
    default:
        handleError(std::wstring(L"wrong tag"));
        return false;
        break;
    }
    return true;
}

bool nbtCompound::serialize(cbool &write, std::wstring &s)
{
    if (write)
    {
        s = (dataTag == nbtDataTag::tagCompound) ? std::wstring(L"{") : std::wstring(L"[");
        for (size_t i = 0; i < children.size(); i++)
        {
            if (dataTag == nbtDataTag::tagCompound)
            {
                s += children[i]->name + std::wstring(L": ");
            }
            std::wstring childValue;
            if (!children[i]->serialize(write, childValue))
                return false;
            s += childValue;
            if (i + 1 < children.size())
            {
                s += L',';
            }
            s += L' ';
        }
        s += (dataTag == nbtDataTag::tagCompound) ? std::wstring(L"}") : std::wstring(L"]");
    }
    else
    {
        if (s.size() > 1)
        {
            s = s.substr(1, s.size() - 2);
            const std::wstring skip = std::wstring(L"\"\"[]{}");
            wstringContainer container = split_string(s, std::wstring(L","), skip);
            children.clear();
            for (const std::wstring &stringPart : container)
            {
                const std::wstring &trimmedString = trim_copy(stringPart);
                if (trimmedString.size())
                {
                    csize_t colonIndex = find(trimmedString, 0, std::wstring(L":"), skip);
                    const std::wstring name =
                        colonIndex == std::wstring::npos ? std::wstring() : trim_copy(trimmedString.substr(0, colonIndex));
                    std::wstring value = trim_copy(trimmedString.substr(colonIndex + 1));
                    const nbtDataTag &tag = getTag(value);
                    nbtData *compound = createNBTData(name, tag);
                    if (!compound->serialize(write, value))
                        return false;
                    children.push_back(compound);
                }
            }
        }
    }
    return true;
}

nbtDataTag nbtCompound::getTag(const std::wstring &value)
{
    if (value.size())
    {
        csize_t &endIndex = value.size() - 1;

        cbool hasInt8Type = value[endIndex] == L'b' ||
                            ((value.length() >= 4) &&
                             ((value.substr(value.length() - 4) == std::wstring(L"true")) ||
                              ((value.length() >= 5) &&
                               value.substr(value.length() - 5) == std::wstring(L"false"))));
        cbool hasInt64Type = value[endIndex] == L'l';
        if (value[0] == L'{')
        {
            return nbtDataTag::tagCompound;
        }
        else if (value[0] == L'[')
        {
            return nbtDataTag::tagList;
        }
        else if (value[0] == L'\"')
        {
            return nbtDataTag::tagUTF8String;
        }
        else if (value.find(L',') != std::wstring::npos)
        {
            if (hasInt8Type)
            {
                return nbtDataTag::tagSignedInt8Array;
            }
            else if (hasInt64Type)
            {
                return nbtDataTag::tagSignedInt64Array;
            }
            else
            {
                return nbtDataTag::tagSignedInt32Array;
            }
        }
        else if (hasInt8Type)
        {
            return nbtDataTag::tagSignedInt8;
        }
        else if (hasInt64Type)
        {
            return nbtDataTag::tagSignedInt64;
        }
        else if (value[endIndex] == L's')
        {
            return nbtDataTag::tagSignedInt16;
        }
        else if (value[endIndex] == L'f')
        {
            return nbtDataTag::tagFP32;
        }
        else if (value.find(L'.') != std::wstring::npos)
        {
            return nbtDataTag::tagFP64;
        }
        else
        {
            return nbtDataTag::tagSignedInt32;
        }
    }
    return nbtDataTag::tagUTF8String;
}

nbtCompound nbtCompound::fromString(std::wstring stringifiedNBT)
{
    nbtCompound compound = nbtCompound(std::wstring(L"setCompound"), nbtDataTag::tagCompound);
    std::wstring modifyableString = stringifiedNBT;
    compound.serialize(false, modifyableString);
    return compound;
}

bool nbtCompound::compare(const nbtData &other) const
{
    const nbtCompound &compareCompound = (const nbtCompound &)other;

    if (compareCompound.children.size() != children.size())
    {
        return false;
    }

    for (nbtData *child : children)
    {
        for (nbtData *compareChild : compareCompound.children)
        {
            if ((compareChild->name == child->name) && (compareChild->dataTag == child->dataTag))
            {
                if (child->compare(*compareChild))
                {
                    goto foundChild;
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    foundChild:;
    }

    return true;
}

bool nbtCompound::serialize(nbtCompound *&compound, cbool &write, const stdPath &path)
{
    // this file should be compressed
    ////https://zlib.net/

    if (write)
    {
        std::stringstream uncompressedStream = std::stringstream(getOpenMode(write));

        auto iFace = createStreamInterface(uncompressedStream);
        streamSerializer s = streamSerializer(iFace, write, std::endian::big);

        serialize(compound, s);

        const std::string &uncompressedResult = uncompressedStream.str();

        const std::string &compressedResult = gzip::compress(uncompressedResult.c_str(),
                                                             uncompressedResult.size());

        writealltext(path, compressedResult);
    }
    else
    {
        // check if compressed

        bool compressed;

        std::fstream stream;
        stream.open(path, getOpenMode(write));
        if (stream.good())
        {
            auto iFace = createStreamInterface(stream);
            streamSerializer s = streamSerializer(iFace, write, std::endian::big);

            constexpr int8_t magicNumbers[2] = {0x1f, (int8_t)0x8b};

            int8_t checkNumbers[2]{};

            if (!s.serialize(checkNumbers[0]))
            {
                return false;
            }
            s.serialize(checkNumbers[1]);

            compressed =
                (checkNumbers[0] == magicNumbers[0]) &&
                (checkNumbers[1] == magicNumbers[1]);
            // stream will close automatically when going out of scope
        }
        else
        {
            return false;
        }

        const std::string &compressedData = readAllText(path);
        const std::string &uncompressedData = compressed ? gzip::decompress(compressedData.c_str(),
                                                                            compressedData.size())
                                                         : compressedData;

        // std::string uncompressedData2 = replace(uncompressedData, std::string(1, '\0'), std::string("<<WATCH OUT>>"));

        // output(stringToWString(uncompressedData2));

        std::stringstream uncompressedStream = std::stringstream(uncompressedData,
                                                                 getOpenMode(write));

        auto iFace = createStreamInterface(uncompressedStream);

        streamSerializer s = streamSerializer(iFace, write, std::endian::big);

        // std::string back = std::string(16000, '#');

        // s.serialize(&back[0], back.length());

        // back = replace(back, std::string(1, '\0'), std::string("<<WATCH OUT>>"));

        // output(stringToWString(back) + L"\n\n");

        serialize(compound, s);
    }

    return true;
}

bool nbtCompound::serialize(nbtCompound *&compound, streamSerializer &s)
{
    nbtDataTag dataTag;
    if (s.write)
    {
        dataTag = compound->dataTag;
    }
    else
    {
        compound = new nbtCompound(std::wstring());
    }

    s.serialize(dataTag);
    serializeChild(s, (nbtData *&)compound, dataTag);
    return true;
}

void nbtCompound::serializeChild(const streamSerializer &s, nbtData *&child,
                                 const nbtDataTag &childTag)
{
    if (s.write)
    {
        s.serializeWStringAsString<int16_t>(child->name);
    }
    else
    {
        std::wstring childName;
        if (childName == std::wstring(L"inventor\x1 slots"))
        {
            throw "found";
        }
        s.serializeWStringAsString<int16_t>(childName);
        child = createNBTData(childName, childTag);
    }
    child->serialize(s);
}

nbtData *nbtCompound::createNBTData(const std::wstring &name, const nbtDataTag &dataTag)
{
    nbtData *resultingData = nullptr;
    switch (dataTag)
    {
    case nbtDataTag::tagCompound:
    case nbtDataTag::tagList:
    {
        resultingData = new nbtCompound(name, dataTag);
    }
    break;
    case nbtDataTag::tagSignedInt8:
    {
        resultingData = new nbtValue<int8_t>(name, dataTag);
    }
    break;
    case nbtDataTag::tagSignedInt16:
    {
        resultingData = new nbtValue<int16_t>(name, dataTag);
    }
    break;
    case nbtDataTag::tagSignedInt32:
    {
        resultingData = new nbtValue<int32_t>(name, dataTag);
    }
    break;
    case nbtDataTag::tagSignedInt64:
    {
        resultingData = new nbtValue<int64_t>(name, dataTag);
    }
    break;
    case nbtDataTag::tagFP32:
    {
        resultingData = new nbtValue<float>(name, dataTag);
    }
    break;
    case nbtDataTag::tagFP64:
    {
        resultingData = new nbtValue<double>(name, dataTag);
    }
    break;
    case nbtDataTag::tagSignedInt8Array:
    {
        resultingData = new nbtDataArray<int8_t>(name, dataTag);
    }
    break;
    case nbtDataTag::tagSignedInt32Array:
    {
        resultingData = new nbtDataArray<int32_t>(name, dataTag);
    }
    break;
    case nbtDataTag::tagSignedInt64Array:
    {
        resultingData = new nbtDataArray<int64_t>(name, dataTag);
    }
    break;
    case nbtDataTag::tagUTF8String:
    {
        resultingData = new nbtValue<std::string>(name, dataTag);
    }
    break;
    default:
        handleError(std::wstring(L"invalid data type"));
        resultingData = nullptr;
        break;
    }

    return resultingData;
}

nbtCompound::~nbtCompound()
{
    for (nbtData *child : children)
    {
        delete child;
    }
}
