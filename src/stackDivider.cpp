
#include "stackDivider.h"
#include "itemData.h"
#include "array/arrayFunctions/sortedArray.h"

bool stackDivider::addStack(itemStack &s) {
    if (!stackHolding.addStack(s))return false;
    originalStack = stackHolding.clone<itemStack>();//copy stack
    return true;
}

bool stackDivider::interactWith(itemStack *candicate) {
    if ((!candicate->count || candicate->compare(originalStack)) && originalStack.count) {
        cint &maxStackSize = itemList[originalStack.stackItemID]->maxStackSize;
        cint &room = maxStackSize - candicate->count;
        if (room <= 0) {
            return false;
        }


        std::vector<size_t> roomPerStack = std::vector<size_t>(divideOver.size());
        //divide evenly
        for (size_t i = 0; i < divideOver.size(); i++) {
            //check if the stack still has the same item type stored in it
            if (divideOver[i]->compare(originalStack)) {
                //retrieve stacks (add them back up to stackholding)
                stackHolding.addStack(*divideOver[i], (int)amountsDivided[i]);
                roomPerStack[i] = maxStackSize - divideOver[i]->count;
            }
        }
        roomPerStack.push_back(room);
        divideOver.push_back(candicate);

        size_t total = stackHolding.count;

        std::vector<size_t> newAmounts = divideOverArray(total, roomPerStack);
        for (size_t i = 0; i < divideOver.size(); i++) {
            divideOver[i]->addStack(stackHolding, (int)newAmounts[i]);
        }
        amountsDivided = newAmounts;
    } else {
        if (candicate->count) {
            if (divideOver.size()) {
                return false;
            }
        }
        //swap slots (also works if occupying slot is empty)
        std::swap(*candicate, stackHolding);
        originalStack = stackHolding;
        pickingUp = true;
    }

    return true;
}
