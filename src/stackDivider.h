#pragma once
#include <vector>
#include "itemStack.h"


//caution! stacks might change while dividing!!!
struct stackDivider {
    std::vector<itemStack*> divideOver = std::vector<itemStack*>();
    itemStack stackHolding = itemStack();
    itemStack originalStack = itemStack();
    bool pickingUp = false;
    std::vector<size_t> amountsDivided = std::vector<size_t>();
    bool addStack(itemStack& s);
    bool interactWith(itemStack* candicate);
};
