#pragma once
#include <tuple>
#include <stddef.h>
#include "array/wstringFunctions.h"
#ifndef _MSC_VER
#include <cxxabi.h>  // needed for abi::__cxa_demangle
#endif

template<size_t typeIndex, typename... types> using nthTypeOf =
typename std::tuple_element<typeIndex, std::tuple<types...>>::type;

template<typename... types> using firstTypeOf = nthTypeOf<0, types...>;

//#include <algorithm>
//#include <array>
//#include <cstddef>
//#include <string_view>
//
//namespace impl
//{
//    template <typename T>
//    [[nodiscard]] constexpr std::string_view RawTypeName()
//    {
//#ifndef _MSC_VER
//        return __PRETTY_FUNCTION__;
//#else
//        return __FUNCSIG__;
//#endif
//    }
//
//    struct TypeNameFormat
//    {
//        std::size_t junk_leading = 0;
//        std::size_t junk_total = 0;
//    };
//
//    constexpr TypeNameFormat type_name_format = [] {
//        TypeNameFormat ret;
//        std::string_view sample = RawTypeName<int>();
//        ret.junk_leading = sample.find("int");
//        ret.junk_total = sample.size() - 3;
//        return ret;
//        }();
//        static_assert(type_name_format.junk_leading != std::size_t(-1), "Unable to determine the type name format on this compiler.");
//
//        template <typename T>
//        static constexpr auto type_name_storage = [] {
//            std::array<char, RawTypeName<T>().size() - type_name_format.junk_total + 1> ret{};
//            std::copy_n(RawTypeName<T>().data() + type_name_format.junk_leading, ret.size() - 1, ret.data());
//            return ret;
//            }();
//}
//
//template <typename T>
//[[nodiscard]] constexpr std::string_view TypeName()
//{
//    return { impl::type_name_storage<T>.data(), impl::type_name_storage<T>.size() - 1 };
//}
//
//template <typename T>
//[[nodiscard]] constexpr const char* TypeNameCstr()
//{
//    return impl::type_name_storage<T>.data();
//}

inline std::wstring demangle(const char* abiName)
{

#ifdef _MSC_VER
	std::wstring retval = stringToWString(abiName);
#else
	int status;
	char* ret = abi::__cxa_demangle(abiName, 0, 0, &status);

	/* NOTE: must free() the returned char when done with it! */
	std::wstring retval = stringToWString(ret);
	free((void*)ret);
	//retval.reset( (char *)ret, [](char *mem) { if (mem) free((void*)mem); } );

#endif // !_MSC_VER
	return retval;

}

template<typename t>
inline std::wstring getClassName(const t& instance)
{
	std::wstring name = demangle(typeid(instance).name());
	csize_t& spacePos = name.rfind(L' ');
	return spacePos == std::wstring::npos ? name : name.substr(spacePos);
}