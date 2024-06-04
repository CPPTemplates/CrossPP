#include "interaction.h"
#include "array/wstringFunctions.h"
#include "globalFunctions.h"

//process events
//return if to continue the program
//bool doEvents()
//{
//	MSG msg;
//	BOOL result;
//
//	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
//	{
//		result = ::GetMessage(&msg, NULL, 0, 0);
//		if (result == 0) // WM_QUIT
//		{
//			::PostQuitMessage((int)msg.wParam);
//			return false;
//		}
//		else if (result == -1)
//		{
//			return false;
//			// Handle errors/exit application, etc.
//		}
//		else
//		{
//			::TranslateMessage(&msg);
//			::DispatchMessage(&msg);
//		}
//	}
//	return true;
//}
//
//bool GetClipboardText(std::wstring& result)
//{
//	if (!OpenClipboard(NULL))
//		return false;
//	const char* stringText = (const char*)GetClipboardData(CF_TEXT);
//	if (stringText == nullptr)
//	{
//		CloseClipboard();
//		return false;
//	}
//	result = stringToWString(std::string(stringText));
//
//	CloseClipboard();
//	return true;
//}
