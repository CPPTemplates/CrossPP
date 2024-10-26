#pragma once
#include "nbt/nbtSerializer.h"
#include "math/graphics/color/color.h"

template <typename t, fsize_t axisCount>
inline bool serializeNBTValue(nbtSerializer &s, const std::wstring &memberName, vectn<t, axisCount> &vector)
{
    if (s.push<nbtDataTag::tagCompound>(memberName))
    {
        for (fsize_t i = 0; i < axisCount; i++)
        {
            s.serializeValue(axisNames[i], vector[i]);
        }

        s.pop();
        return true;
    }
    else
    {
        return false;
    }
}

template <typename t, fsize_t axisCount>
inline bool serializeNBTValue(nbtSerializer &s, const std::wstring &memberName, colortn<t, axisCount> &vector)
{
    return serializeNBTValue(s, memberName, (vectn<t, axisCount> &)vector);
}