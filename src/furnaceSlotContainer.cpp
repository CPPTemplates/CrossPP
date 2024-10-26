#include "entity.h"
#include "furnaceSlotContainer.h"
#include "resourcePack.h"
#include "folderList.h"
#include "furnaceData.h"
#include "human.h"
#include "furnaceRecipe.h"
#include "dimension.h"
#include "stackDivider.h"

furnaceSlotContainer::furnaceSlotContainer() {
    uiTexture = loadTextureFromResourcePack(containerTextureFolder / std::wstring(L"furnace.png"));
    containers.push_back(furnaceInputSlot = new uiSlotContainer(cveci2(56, 133), nullptr));
    containers.push_back(furnaceOutputSlot = new uiSlotContainer(cveci2(116, 116), nullptr));
    containers.push_back(furnaceFuelSlot = new uiSlotContainer(cveci2(56, 97), nullptr));
    //hotbar and inventory will be linked up
    containers.push_back(hotbarSlots);
    containers.push_back(inventorySlots);
}

bool furnaceSlotContainer::addStack(itemStack &stack) {
    return furnaceInputSlot->addStack(stack) || furnaceFuelSlot->addStack(stack);
}

void furnaceSlotContainer::drawExtraData(cmat3x3 &transform, const texture &renderTarget) {
    //draw fire and progress bar
    //draw fire
    if (selectedFurnaceData->ticksFuelBurned) {
        fp burnProgress =
                selectedFurnaceData->ticksFuelBurned / (fp) selectedFurnaceData->ticksFuelToBurn;
        inventory::drawExtraData(crectanglei2(176, 242, 15, (int) (14 * (1.0 - burnProgress))),
                                 veci2(56, 116), transform, renderTarget);
    }
    //draw progress bar
    if (selectedFurnaceData->ticksCooked) {
        fp cookProgress = selectedFurnaceData->ticksCooked /
                          (fp) selectedFurnaceData->currentRecipe->cookingTime;
        inventory::drawExtraData(crectanglei2(176, 224, (int) (24 * cookProgress), 18),
                                 veci2(79, 114), transform, renderTarget);
    }
}

void furnaceSlotContainer::clickedOnItem(cmb &button, stackDivider &divider,
                                         uiSlotContainer *selectedSlotContainer,
                                         veci2 selectedSlot) {
    human *currentHuman = (human *) linkedPlayer;
    if (selectedSlotContainer == furnaceOutputSlot) {
        if (divider.divideOver.empty()) {

            itemStack *s = selectedSlotContainer->getSlot(selectedSlot);
            divider.addStack(*s);
            currentHuman->addExperience(
                    roundRandom(currentRandom, selectedFurnaceData->collectedExperience));
            selectedFurnaceData->collectedExperience = 0;
        }
    } else {
        inventory::clickedOnItem(button, divider, selectedSlotContainer, selectedSlot);
        if (selectedSlotContainer == furnaceInputSlot) {
            furnaceRecipe *newRecipe = findRecipe(furnaceRecipes,
                                                  furnaceInputSlot->linkedContainer);  // getFurnaceRecipe(furnaceInputSlot->linkedContainer);
            if (selectedFurnaceData->currentRecipe != newRecipe) {
                selectedFurnaceData->currentRecipe = newRecipe;
                selectedFurnaceData->ticksCooked = 0;
            }
        }
    }
    currentHuman->selectedContainerContainer->addUpdatePosition(
            currentHuman->selectedContainerPosition);
}

furnaceSlotContainer::~furnaceSlotContainer() {
    delete furnaceInputSlot;
    delete furnaceOutputSlot;
    delete furnaceFuelSlot;
    delete hotbarSlots;
    delete inventorySlots;
}