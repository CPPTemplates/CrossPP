#include "wstringFunctions.h"

//https://stackoverflow.com/questions/4654636/how-to-determine-if-a-wstring-is-a-number-with-c
//base radix
bool convertToLong(const std::wstring& line, long& number, cint& base)
{
	wchar_t* p;
	number = wcstol(line.c_str(), &p, base);
	return *p == 0;
}
bool convertToInt(const std::wstring& line, int& number, cint& base)
{
	//dont use stoi as it will throw
	long l;
	if (convertToLong(line, l, base))
	{
		number = l;
		return true;
	}
	else
	{
		return false;
	}
}
//base 10
bool convertToDouble(const std::wstring& line, fp& number)
{
	//weirdly "" is converted to 0
	if (line == std::wstring())
	{
		return false;
	}
	else
	{
		wchar_t* p;
		number = wcstod(line.c_str(), &p);
		return *p == 0;
	}
}
std::wstring convertToLowerCase(const std::wstring& s)
{
	std::wstring result = s;
	convertToLowerCase(result);
	return result;
}
void convertToLowerCase(std::wstring& s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](const letter& l) { return std::tolower(l); });
}
letter convertToLowerCase(const letter& s)
{
	return (letter)std::tolower(s);
}

//https://stackoverflow.com/questions/17386766/how-do-i-get-a-char-obtained-by-holding-shift
//this function assumes that you changed keyboard layout to US english
//bool getLetter(cvk& keyCode, cbool& shift, letter& result)
//{
//
//	short vkKeyScanResult = VkKeyScan(keyCode);
//	
//	if (vkKeyScanResult == -1)
//		return false;// no key translates to input character
//	
//	// vkKeyScanResult & 0xff is the base key, without any modifiers
//	uint code = (uint)vkKeyScanResult & 0xff;
//	
//	byte* b = new byte[256]();//initialize all keys to not pressed
//	if (shift)
//	{
//		// set shift key pressed
//		b[VK_SHIFT] = 0x80;
//	}
//	
//	ushort r;
//	// return value of 1 expected (1 character copied to r)
//	if (1 != ToAscii(code, code, b, &r, 0))
//		return false;//could not translate modified state
//	
//	delete[] b;
//	result = (char)r;
//	return true;
//	//letter result = keyCode;
//	//std::wstring from = L"abcdefghijklmnopqrstuvwxyz1234567890";
//	//std::wstring to = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()";
//	//size_t index = from.find(keyCode);
//	//if (index == std::wstring::npos)
//	//{
//	//	switch (keyCode)
//	//	{
//	//	case VK_OEM_MINUS:
//	//		result = L'-';
//	//		return true;
//	//	case VK_OEM_COMMA:
//	//		result = L',';
//	//		return true;
//	//	case VK_OEM_PERIOD:
//	//		result = L'.';
//	//		return true;
//	//	case VK_OEM_PLUS:
//	//		result = L'+';
//	//		return true;
//	//	default:
//	//		return false;
//	//	}
//	//}
//	//else {
//	//	result = shift ? to[index] : keyCode;
//	//	return true;
//	//}
//}