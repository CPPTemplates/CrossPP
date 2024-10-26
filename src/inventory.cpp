#include "inventory.h"
#include "human.h"
#include "stackDivider.h"

void inventory::clickedOnItem(cmb &button, stackDivider &divider,
                              uiSlotContainer *selectedSlotContainer, veci2 selectedSlot)
{
    itemStack *occupyingSlot = selectedSlotContainer->linkedContainer->getSlot(
        selectedSlot);
    if (canAddStack(selectedSlotContainer, divider))
    {
        divider.interactWith(occupyingSlot);
    }

    // if (divider.originalStack.count) {
    //     if (canAddStack(selectedSlotContainer, divider)) {
    //         if (!onAndroid && button == (mb) sf::Mouse::Right) {
    //             //selectedSlotContainer->linkedContainer->getSlot(selectedSlot)->addStack(
    //             //        divider, 1);
    //         } else {
    //             divider.interactWith(occupyingSlot);
    //             //if (occupyingSlot->stackItemID == divider.originalStack.stackItemID &&
    //             //    occupyingSlot->compare(divider.originalStack)) {
    //             //    occupyingSlot->addStack(stackHolding);
    //             //} else {
    //             //    if (occupyingSlot->count) {
    //             //        //can't divide original stack when swapping items
    //             //        slotsDividingOver.clear();
    //             //    }
    //             //    //swap slots (also works if occupying slot is empty)
    //             //    std::swap(*occupyingSlot, stackHolding);
    //             //}
    //         }
    //     }
    // } else {
    //     itemStack *s = selectedSlotContainer->getSlot(selectedSlot);
    //     divider.stackHolding->addStack(*s);
    // }
}

void inventory::drawExtraData(crectanglei2 &textureRect, cveci2 &offset, cmat3x3 &transform, const texture &renderTarget) const
{
    fillTransparentRectangle(crectangle2(textureRect), mat3x3::cross(transform, mat3x3::translate(cvec2(offset - textureRect.pos0))), *uiTexture, renderTarget);
}

void inventory::drawExtraData(cmat3x3 &transform, const texture &renderTarget)
{
}

inventory::~inventory()
{
    for (uiSlotContainer *container : containers)
    {
        delete container;
        container = nullptr;
    }
}

void inventory::mouseDown(cveci2 &pixelPosition, cmb &button, stackDivider &divider)
{
    human *currentHuman = (human *)linkedPlayer;
    // check which inventory slot is clicked on
    // convert to 'hotbar slot space'
    veci2 selectedItemPosition;
    uiSlotContainer *selectedSlotContainer = getSlotContainer(pixelPosition, selectedItemPosition);
    if (selectedSlotContainer)
    {
        clickedOnItem(button, divider, selectedSlotContainer, selectedItemPosition);
    }
    else if (divider.stackHolding.count)
    { // not dividing anything, else stackholding would be empty
        // outside of the slotcontainers
        // drop on the ground
        currentHuman->drop(divider.stackHolding);
        divider = stackDivider();
    }
}

void inventory::drawToolTips(cveci2 &mouseTexturePosition, cveci2 &mousePositionPixels,
                             const texture &renderTarget)
{
    // check which inventory slot is hovered above
    // convert to 'hotbar slot space'
    veci2 selectedItemPosition;
    uiSlotContainer *container = getSlotContainer(mouseTexturePosition, selectedItemPosition);
    if (container)
    {
        itemStack *stack = container->linkedContainer->getSlot(selectedItemPosition);
        if (stack->count)
        {
            stack->drawToolTips(mousePositionPixels, renderTarget);
        }
    }
}

bool inventory::canAddStack(uiSlotContainer *containerToAddTo, stackDivider &s)
{
    return true;
}

uiSlotContainer *inventory::getSlotContainer(cveci2 &mousePositionPixels, veci2 &slotPosition)
{
    // check which inventory slot is hovered above
    // convert to 'hotbar slot space'
    for (uiSlotContainer *const &container : containers)
    {
        if (container->linkedContainer->getSlot(mousePositionPixels,
                                                container->containerPixelPosition, inventorySpacing,
                                                inventoryItemDisplaySize, slotPosition))
        {
            return container; // no other slotcontainer will have items at the same place
        }
    }
    return nullptr;
}

void inventory::mouseMove(cveci2 &pixelPosition, cmb &button, stackDivider &divider)
{
    if (!divider.pickingUp)
    {
        veci2 selectedItemPosition;
        uiSlotContainer *selectedSlotContainer = getSlotContainer(pixelPosition, selectedItemPosition);
        if (selectedSlotContainer)
        {
            itemStack *s = selectedSlotContainer->getSlot(selectedItemPosition);
            if (s && std::find(divider.divideOver.begin(), divider.divideOver.end(), s) ==
                         divider.divideOver.end())
            {
                // new stack to potentially add
                clickedOnItem(button, divider, selectedSlotContainer, selectedItemPosition);
            }
        }
    }
}
