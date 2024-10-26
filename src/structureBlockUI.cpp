#include "human.h"
#include "structureBlockUI.h"
#include "structureBlockData.h"
#include "dimension.h"
#include "gameControl.h"
#include "constants/vectorConstants.h"

structureBlockUI::structureBlockUI()
{
    structureBlockLabel->text = std::wstring(L"Structure Block");
    nameLabel->text = std::wstring(L"Structure Name");
    modeButton->text = std::wstring(L"Save Mode");
    doneButton->text = std::wstring(L"Done");
    cancelButton->text = std::wstring(L"Cancel");

    addChildren({structureBlockLabel,
                 nameLabel,
                 nameTextbox,

                 saveUI,
                 loadUI,

                 modeLabel,
                 modeButton,
                 doneButton,
                 cancelButton});
}

void structureBlockUI::serializeWithTemporaryData(cbool &write)
{
    serialize(nameTextbox->text, temporaryStructureBlockData->structureName, write);
    saveUI->serializeWithTemporaryData(write);
    loadUI->serializeWithTemporaryData(write);
}

void structureBlockUI::layout(crectanglei2 &rect)
{
    form::layout(rect);
    cint structureBlocklabelWidth =
        (int)defaultTheme().font->measureStringSize(measureStringMaximumSize,
                                                    structureBlockLabel->text)
            .x +
        defaultTheme().borderSize * 2;
    cint middle = rect.w / 2;
    int descendingY = rect.h - buttonSize.y;

    structureBlockLabel->layout(crectanglei2(middle - structureBlocklabelWidth / 2, descendingY,
                                             structureBlocklabelWidth, buttonSize.y));
    descendingY -= buttonSize.y * 2;
    nameLabel->layout(crectanglei2(0, descendingY, rect.w, buttonSize.y));
    descendingY -= buttonSize.y;
    nameTextbox->layout(crectanglei2(0, descendingY, rect.w, buttonSize.y));

    int ascendingY = 0;
    doneButton->layout(crectanglei2(0, ascendingY, middle, buttonSize.y));
    cancelButton->layout(crectanglei2(middle, ascendingY, rect.w - middle, buttonSize.y));

    ascendingY += buttonSize.y;
    modeButton->layout(crectanglei2(0, ascendingY, smallRightButtonSize, buttonSize.y));
    ascendingY += buttonSize.y;
    modeLabel->layout(crectanglei2(0, ascendingY, 0, buttonSize.y));
    ascendingY += buttonSize.y;

    crectanglei2 modeRect = crectanglei2(0, ascendingY, rect.w, descendingY - ascendingY);
    saveUI->layout(modeRect);
    loadUI->layout(modeRect);
}

void structureBlockUI::render(cveci2 &position, const texture &renderTarget)
{
    saveUI->visible = false;
    loadUI->visible = false;

    switch (temporaryStructureBlockData->mode)
    {
    case structureBlockMode::saveMode:
    {
        modeButton->text = std::wstring(L"Save");
        modeLabel->text = std::wstring(L"Save Mode - Write To File");
        saveUI->visible = true;
    }
    break;
    case structureBlockMode::loadMode:
    {
        modeButton->text = std::wstring(L"Load");
        modeLabel->text = std::wstring(L"Load Mode - Load From File");
        loadUI->visible = true;
    }
    break;
    case structureBlockMode::cornerMode:
    {
        modeButton->text = std::wstring(L"Corner");
        modeLabel->text = std::wstring(L"Corner Mode - Placement And Size Marker");
    }
    default:
        break;
    }
    temporaryStructureBlockData->structureName = nameTextbox->text;
    cint modeLabelWidth = (int)defaultTheme().font->measureStringSize(measureStringMaximumSize,
                                                                      modeLabel->text)
                              .x +
                          defaultTheme().borderSize * 2;
    modeLabel->layout(
        crectanglei2(modeLabel->rect.x, modeLabel->rect.y, modeLabelWidth, modeLabel->rect.h));
    renderChildren(position, renderTarget);
}

void structureBlockUI::mouseDown(cveci2 &position, cmb &button)
{
    control *highestChild = getHighestChild(position);
    if (highestChild == modeButton)
    {
        temporaryStructureBlockData->mode = (structureBlockMode)((int)temporaryStructureBlockData->mode + 1);
        if (temporaryStructureBlockData->mode > structureBlockMode::cornerMode)
        {
            temporaryStructureBlockData->mode = structureBlockMode::saveMode;
        }
    }
    else if (highestChild == doneButton)
    {
        // save
        serializeWithTemporaryData(true);
        const human &player = *((gameControl *)parent)->player;
        blockData *selectedBlockData = player.selectedContainerContainer->getBlockData(
            player.selectedContainerPosition);
        *(structureBlockData *)selectedBlockData = *temporaryStructureBlockData;
        delete temporaryStructureBlockData;
        parent->switchVisibleChild(nullptr);
    }
    else if (highestChild == cancelButton)
    {
        delete temporaryStructureBlockData;
        parent->switchVisibleChild(nullptr);
    }
    form::mouseDown(position, button);
}