#pragma once
#include "nbt/nbtSerializer.h"
#include "math/uuid.h"
inline bool serializeNBTValue(nbtSerializer &s, const std::wstring &memberName, uuid &id)
{
    if (s.push<nbtDataTag::tagCompound>(memberName))
    {
        s.serializeValue(std::wstring(L"a"), (int &)id.idInts[0]);
        s.serializeValue(std::wstring(L"b"), (int &)id.idInts[1]);
        s.serializeValue(std::wstring(L"c"), (int &)id.idInts[2]);
        s.serializeValue(std::wstring(L"d"), (int &)id.idInts[3]);
        s.pop();
        return true;
    }
    else
    {
        return false;
    }
}