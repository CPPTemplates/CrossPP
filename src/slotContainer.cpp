#include "slotContainer.h"
#include "world.h"
#include "include/optimization/handleError.h"
bool slotContainer::addStack(itemStack& stack)
{
	handleError(std::wstring(L"not implemented"));
	return false;
}

bool slotContainer::substractStack(itemStack& stack)
{
	currentWorld->currentChat.addLine(std::wstring(L"substraction not implemented"));
	return false;
}

void slotContainer::serializeValue(nbtSerializer& s)
{
	handleError(std::wstring(L"not implemented"));
}

void slotContainer::clearData()
{
}