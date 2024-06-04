#pragma once
#include <string>
//#include <stringapiset.h>
#include "globalFunctions.h"
#include <filesystem>
#include <algorithm>
#include <vector>
#include <locale>
#include <codecvt>
#include <sstream>

typedef std::vector<std::wstring> wstringContainer;
typedef std::vector<std::string> stringContainer;

//litteraly copies chars to letters
//https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar
//cross-platform way:
//https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
//cross-platform trick:
//https://www.reddit.com/r/cpp/comments/hlpdec/is_there_a_standard_way_to_convert_between_string/
inline std::wstring stringToWString(const std::string& s)
{
	return std::filesystem::path(s).wstring();
	//return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(s);
	//corrupts memory
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	//return converter.from_bytes(s);//DONT EVER USE converterX.from_bytes(s) it will cost gigabytes of memory!
	//std::wstring wsTmp(s.begin(), s.end());
	//return wsTmp;
	//-1 because there is a '\0' charachter
	//int wCharCount = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0) - 1;
	//std::wstring r((size_t)wCharCount, L'#');//make a new string with the measured length and fill it with '#'
	//MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &r[0], wCharCount);//fill the string with the new wstring
	//return r;
	//return std::wstring(s.begin(), s.end());
	//using convert_typeX = std::codecvt_utf8<wchar_t>;
    //std::wstring_convert<convert_typeX, wchar_t> converterX;

    //return converterX.from_bytes(s);
}
//litteraly copies letters to chars
inline std::string WStringToString(const std::wstring& s)
{
	return std::filesystem::path(s).string();
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	//return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(s);
	//return std::string_convert<std::codecvt_utf16<char>>().from_bytes(s);
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	//return converter.to_bytes(s);
	//std::string sTmp(s.begin(), s.end());
	//return sTmp;
	//-1 because there is a '\0' charachter
	//int charCount = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL, 0, NULL, NULL) - 1;
	//std::string r((size_t)charCount, '#');//make a new string with the measured length and fill it with '#'
	//WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &r[0], charCount, NULL, NULL);//fill the string with the new wstring
	//return r;
	//std::locale::
	//return std::string(s.begin(), s.end());
    //using convert_typeX = std::codecvt_utf8<wchar_t>;
    //std::wstring_convert<convert_typeX, wchar_t> converterX;
//
    //return converterX.to_bytes(s);
}

inline std::wstring towstring(cfp value, cint digits)
{
	std::wstringstream ss;
	ss << std::fixed << std::setprecision(digits) << value;
	return ss.str();
};

//https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start (in place)
inline static void ltrim(std::wstring& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
inline static void rtrim(std::wstring& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
inline static void trim(std::wstring& s)
{
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
inline static std::wstring ltrim_copy(std::wstring s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
inline static std::wstring rtrim_copy(std::wstring s) {
	rtrim(s);
	return s;
}

// trim from both ends (copying)
inline static std::wstring trim_copy(std::wstring s) {
	trim(s);
	return s;
}

inline bool compareSubstring(const std::wstring& stringToTakeSubstringFrom, const std::wstring& compareTo, const size_t& offset = 0)
{
	return stringToTakeSubstringFrom.compare(offset, compareTo.size(), compareTo) == 0;//no differences accepted
}

inline size_t find(std::wstring str, size_t offset, std::wstring seekfor, std::wstring skip)
{
	while (offset < str.size())
	{
		if (compareSubstring(str, seekfor, offset))
		{
			return offset;
		}
		else
		{
			size_t index = skip.find(str[offset]);
			if (index != std::wstring::npos)
			{
				offset = find(str, offset + 1, std::wstring() + skip[index + 1], skip);
				if (offset == std::wstring::npos)
				{
					return std::wstring::npos;
				}
			}
		}
		offset++;
	}
	return std::wstring::npos;
}
//returns an index to the array
inline size_t FindNearest(std::vector<size_t> positions)
{
	size_t nearestPosition = std::wstring::npos;
	size_t nearestIndex = std::wstring::npos;
	for (size_t currentIndex = 0; currentIndex < positions.size(); currentIndex++)
	{
		size_t position = positions[currentIndex];
		if (position != std::wstring::npos && (position < nearestPosition || nearestPosition == std::wstring::npos))
		{
			nearestPosition = position;
			nearestIndex = currentIndex;
		}
	}
	return nearestIndex;
}
inline std::vector<std::wstring> split_string(const std::wstring& str, const letter& delimiter, const std::wstring& skip)
{
	std::vector<std::wstring> strings;
	size_t start = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		const letter c = str[i];
		if (c == delimiter)
		{
			strings.push_back(str.substr(start, i - start));
			start = i + 1;
		}
		else
		{
			size_t index = skip.find(c, 0);
			if (index != std::wstring::npos)
			{
				i = find(str, i + 1, std::wstring{ skip[index + 1] }, skip);
				if (i == std::wstring::npos) break;
			}
		}
	}
	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(start));
	return strings;
}
inline std::vector<std::wstring> split_string(const std::wstring& str, const std::wstring& delimiter, const std::wstring& skip)
{
	std::vector<std::wstring> strings;
	if (str.size())
	{
		size_t previousIndex = 0;
		while (true)
		{
			csize_t& currentIndex = find(str, previousIndex, delimiter, skip);
			if (currentIndex == std::wstring::npos) {
				strings.push_back(str.substr(previousIndex));
				break;
			}
			else
			{
				strings.push_back(str.substr(previousIndex, currentIndex - previousIndex));
			}
			previousIndex = currentIndex + delimiter.size();
		}
		return strings;
	}
	else
	{
		return strings;
	}
}

inline std::vector<std::wstring> split_string(const std::wstring& str, const std::wstring& delimiter)
{
	std::vector<std::wstring> strings;

	std::wstring::size_type pos = 0;
	std::wstring::size_type prev = 0;
	while ((pos = str.find(delimiter, prev)) != std::wstring::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + delimiter.size();
	}

	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(prev));

	return strings;
}

template<typename charachterType>
inline std::basic_string<charachterType> replace(std::basic_string<charachterType> str, std::basic_string<charachterType> from, std::basic_string<charachterType> to)
{
	size_t lastindex = 0;
	while (true)
	{
		csize_t& newindex = str.find(from, lastindex);
		if (newindex == std::wstring::npos)
		{
			return str;
		}
		else
		{
			str = str.substr(0, newindex) + to + str.substr(newindex + from.length());
			lastindex = newindex + to.length();
		}
	}
}
inline size_t FindLine(const wstringContainer& lines, size_t offset, const std::wstring& seekfor)
{
	while (offset < lines.size())
	{
		if (lines[offset] == seekfor)
			return offset;
		offset++;
	}
	return std::wstring::npos;
}
inline size_t FindLine(wstringContainer lines, size_t offset, std::wstring seekfor, wstringContainer skip)
{
	while (offset < lines.size())
	{
		if (lines[offset] == seekfor)
		{
			return offset;
		}
		else
		{
			csize_t& index = FindLine(skip, 0, lines[offset]);
			if (index != std::wstring::npos)
			{
				offset = FindLine(lines, offset + 1, skip[index + 1], skip);
			}
		}
		offset++;
	}
	return std::wstring::npos;
}

//rounds a number to a specified amount of digits and converts it to a string
template<typename t>
inline std::wstring toWstringDigits(const t& input, cfsize_t& digits)
{
	std::wstringstream stream;
	stream << std::fixed << std::setprecision(digits) << input;
	return stream.str();
}


std::wstring convertToLowerCase(const std::wstring& s);
void convertToLowerCase(std::wstring& s);
letter convertToLowerCase(const letter& s);


bool convertToDouble(const std::wstring& line, fp& number);
bool convertToLong(const std::wstring& line, long& number, cint& base = 10);
bool convertToInt(const std::wstring& line, int& number, cint& base = 10);

inline std::wstring getWord(const wstringContainer& sentenceContainer, csize_t& index)
{
	return index < sentenceContainer.size() ? sentenceContainer[index] : std::wstring();
}