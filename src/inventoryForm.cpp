#include "inventoryForm.h"
#include "inventory.h"
#include "human.h"
#include "dimension.h"
#include "soundList.h"
#include "gameControl.h"
#include "settings.h"
#include "playerSocket.h"
#include "renderData.h"
#include "enchantingTableSlotContainer.h"
#include "craftableSlotContainer.h"

void inventoryForm::linkUp(inventory *newInventoryToDisplay)
{
    inventoryToDisplay = newInventoryToDisplay;
    newInventoryToDisplay->linkedPlayer = ((gameControl *)parent)->player;
    updateScale();
}

void inventoryForm::unLink()
{
    human *currentPlayer = ((gameControl *)parent)->player;
    if ((inventoryToDisplay == (inventory*)currentPlayer->craftingTableSlots) ||
        (inventoryToDisplay == (inventory*)currentPlayer->humanSlots) ||
        (inventoryToDisplay == (inventory*)currentPlayer->smithingTableSlots) ||
        (inventoryToDisplay == (inventory*)currentPlayer->anvilSlots))
    {
        // drop items the crafting table holds
        for (uiSlotContainer *container : ((craftableSlotContainer *)inventoryToDisplay)->craftingInputContainers)
        {
            container->linkedContainer->transfer(currentPlayer, currentPlayer->dimensionIn,
                                                 currentPlayer->position);
        }

        // removes items from the output slot
        ((craftableSlotContainer *)inventoryToDisplay)->recalculateOutputSlot();
    }
    else if (inventoryToDisplay == currentPlayer->enchantmentSlots)
    {
        ((enchantingTableSlotContainer *)inventoryToDisplay)->lapisSlot->linkedContainer->transfer(currentPlayer, currentPlayer->dimensionIn, currentPlayer->position);
        ((enchantingTableSlotContainer *)inventoryToDisplay)->enchantmentSlot->linkedContainer->transfer(currentPlayer, currentPlayer->dimensionIn, currentPlayer->position);
    }
    else if (inventoryToDisplay == (inventory*)currentPlayer->chestSlots)
    {
        const blockID &blockType = currentPlayer->selectedContainerContainer->getBlockID(
            currentPlayer->selectedContainerPosition);
        (isChest(blockType) ? chestCloseSound : barrelCloseSound)->playRandomSound(currentPlayer->selectedContainerContainer, cvec2(currentPlayer->selectedContainerPosition) + 0.5);
    }

    inventoryToDisplay = nullptr;

    visible = false;
    if (parent->focusedChild == this)
    {
        parent->focusChild(nullptr);
    }
}

void inventoryForm::mouseDown(cveci2 &position, cmb &button)
{
    form::mouseDown(position, button);
    holdingMousePos = position;
    mouseButtonPressing = button;
    inventoryToDisplay->mouseDown(cveci2(cvec2(position) / scaleMultiplier), button, divider);
}

void inventoryForm::mouseMove(cveci2 &position, cmb &button)
{
    form::mouseMove(position, button);
    holdingMousePos = position;
    if (mouseButtonPressing != noButton)
    {
        inventoryToDisplay->mouseMove(cveci2(cvec2(position) / scaleMultiplier), mouseButtonPressing, divider);
    }
}

void inventoryForm::mouseUp(cveci2 &position, cmb &button)
{
    control::mouseUp(position, button);
    mouseButtonPressing = noButton;
    divider.divideOver.clear();
    divider.amountsDivided.clear();
    if (divider.stackHolding.count)
    {
        divider.originalStack.count = divider.stackHolding.count;
        divider.pickingUp = false;
    }
    else
    {
        divider.originalStack = itemStack();
    }
}

inventoryForm::inventoryForm() : form()
{
}

void inventoryForm::render(cveci2 &position, const texture &renderTarget)
{
    // update control
    updateScale();
    fillTransparentRectangle((crectangle2)inventoryToDisplay->uiTexturePartToDraw, this->rect,
                             *inventoryToDisplay->uiTexture, renderTarget);

    const gameRenderData &targetData = ((gameControl *)parent)->getRenderData(renderTarget);
    for (const uiSlotContainer *const &c : inventoryToDisplay->containers)
    {
        c->linkedContainer->render(targetData,
                                   position + (vec2)c->containerPixelPosition * scaleMultiplier,
                                   inventorySpacing * scaleMultiplier,
                                   inventoryItemDisplaySize * scaleMultiplier);
    }
    inventoryToDisplay->drawExtraData(mat3x3::fromRectToRect(
                                          crectangle2(cvec2(0), cvec2(inventoryToDisplay->uiTexturePartToDraw.size)),
                                          crectangle2(rect)),
                                      renderTarget);
    // render the original stack, so you can see what you're dividing
    if (divider.originalStack.count)
    {
        divider.originalStack.render(crectangle2(cvec2(position + holdingMousePos) -
                                                     (inventoryItemDisplaySize * 0.5 * scaleMultiplier),
                                                 vec2(inventoryItemDisplaySize) * scaleMultiplier),
                                     targetData);
    }
    else
    {
        inventoryToDisplay->drawToolTips(cveci2(cvec2(holdingMousePos) / scaleMultiplier),
                                         holdingMousePos + position, renderTarget);
    }
}

void inventoryForm::updateScale()
{
    scaleMultiplier = ((fp)parent->rect.size.x / (fp)defaultUIRectangle.size.x) * (((gameControl *)parent)->touchInput ? 0.5 : 0.25) *
                      settings::videoSettings::guiScale; // normally the UI should take up 1/4th of the screen
    rect.size = cveci2(cvec2(inventoryToDisplay->uiTexturePartToDraw.size) * scaleMultiplier);
    rect.moveToCenter(parent->rect);
}
