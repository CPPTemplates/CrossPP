#include "uiSlotContainer.h"
#include "humanSlotContainerUI.h"
#include "itemStack.h"
#include "craftingTableSlotContainer.h"
#include "furnaceSlotContainer.h"
#include "chestSlotContainer.h"
#include "brewingStandSlotContainer.h"
#include "enchantingTableSlotContainer.h"
#include "dispenserSlotContainer.h"
#include "smithingTableSlotContainer.h"
#include "anvilSlotContainer.h"
bool uiSlotContainer::addStack(itemStack& stack)
{
	return linkedContainer->addStack(stack);
}

uiSlotContainer::~uiSlotContainer()
{

	delete linkedContainer;
}