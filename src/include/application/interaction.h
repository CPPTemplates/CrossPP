#pragma once
#include "globalFunctions.h"
#include <string>

bool doEvents();

//bool GetClipboardText(std::wstring& result);

//returns wether this key is pressed
//source:
//https://stackoverflow.com/questions/41606356/catch-ctrlalt-keys-in-wndproc
//inline bool pressed(cushort& key)
//{
//	return GetKeyState(key) & 0x8000;
//}

letter getModifiedShiftKey(const cvk& keyCode);
bool getLetter(cvk& keyCode, cbool& shift, letter& result);