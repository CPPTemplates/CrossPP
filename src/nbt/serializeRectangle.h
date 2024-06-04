#pragma once
#include "nbt/nbtSerializer.h"
#include "math/rectangle/rectangletn.h"
#include "serializeColor.h"
template <typename t>
inline bool serializeNBTValue(nbtSerializer &s, const std::wstring &memberName, rectanglet2<t> &box)
{
    if (s.push<nbtDataTag::tagCompound>(memberName))
    {
        serializeNBTValue(s, std::wstring(L"position"), box.pos0);
        serializeNBTValue(s, std::wstring(L"size"), box.size);
        s.pop();
        return true;
    }
    else
    {
        return false;
    }
}
