#include "idList.h"

//get rid of that 'minecraft:' part
std::wstring removeNameSpace(const std::wstring& name)
{
	int colonIndex = (int)name.find(std::wstring(L":"));
	return colonIndex == -1 ? name : name.substr(colonIndex + 1);
}