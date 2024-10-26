#pragma once
#include "nbt/nbtSerializer.h"
//fallback function
//make sure to have instantiated the templates by for example #include "nbt/serializeColor.h"!
template<typename t>
inline bool serializeNBTValue(nbtSerializer&s,  const std::wstring &memberName, t &value)
{
    return s.serializeValue(memberName, value);
}
// for basic lists, use the serializeVariableArray function
template <typename t>
inline bool serializeNBTValue(nbtSerializer &s, const std::wstring &memberName, std::vector<t> &value)
{
    // CAUTION! this SFINAE construct uses double ::type! basically, it doesn't get the ::type until the end of the evaluation, because not all ::type s are actually defined
    // std::common_type is just a trick to get a struct with ::type being t
    using signedT = std::conditional_t<std::is_integral_v<t>, std::make_signed<t>,
                                       std::conditional_t<std::is_enum_v<t>, std::underlying_type<t>,
                                                          std::conditional_t<std::is_same_v<t, bool>, std::common_type<bool>,
                                                                             std::common_type<t>>>>::type;
    // using signedT = std::conditional_t<std::is_integral_v<t>, std::make_signed_t<t>, t>;
    //  using signedT = std::is_integral_v<t> ? std::make_signed_t<t> : t;
    //constexpr auto dataTag = getListDataTag<signedT *>();
    if constexpr (getListDataTag<signedT *>() != nbtDataTag::tagEnd)
    {
        if (nbtData *currentChild = s.getOrCreateNBTData<signedT *>(memberName))
        {
            if (!s.write)
            {
                value.resize(((nbtDataArray<signedT> *)currentChild)->arraySize);
            }
            size_t count = value.size();
            signedT *ptr = (signedT *)&value[0]; // ptr will not get modified because it's not null
            return s.serializeVariableArray(*currentChild, ptr, count);
        }
    }
    else
    {

        if (s.push<nbtDataTag::tagList>(memberName))
        {
            if (!s.write)
            {
                value = std::vector<t>(s.getChildren().size());
            }
            for (t &it : value)
            {
                serializeNBTValue(s, std::wstring(), it);
            }
            s.pop();
            return true;
        }
    }
    return false;
}

template <typename t>
inline bool serializeListOfSerializables(nbtSerializer &s, const std::wstring &memberName, std::vector<t> &value)
{
    if (s.push<nbtDataTag::tagList>(memberName))
    {
        if (!s.write)
        {
            value = std::vector<t>(s.getChildren().size());
        }
        //size_t i = 0;
        for (t &it : value)
        {
            if (s.push())
            {
                it.serializeValue(s);
                s.pop();
            }
        }
        s.pop();
        return true;
    }
    return false;
}