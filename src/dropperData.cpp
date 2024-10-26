#include "dropperData.h"
bool dropperData::dispenseItem(const itemStack& stack, tickableBlockContainer* containerIn, cveci2& position)
{
	return dropItem(stack, containerIn, position);
}