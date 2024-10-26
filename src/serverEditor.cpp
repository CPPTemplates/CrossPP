#include "serverEditor.h"
#include "keyID.h"
#include "serverSelector.h"
#include "include/application/theme.h"

serverEditor::serverEditor() {
    addChildren({nameLabel, nameTextBox, ipAddressLabel, ipAddressTextBox, doneButton});
    addEventHandlers(&serverEditor::doneButtonMouseDown, doneButton->onMouseDown);
}

void serverEditor::layout(crectanglei2 &newRect) {
    form::layout(newRect);
    //center controls
    cint offsetFromCenter = 0x3;
    cint middleX = (rect.w - buttonSize.x) / 2;
    //cint leftSideX = rect.w / 2 - buttonSize.x - offsetFromCenter;
    //cint rightSideX = rect.w / 2 + offsetFromCenter;
    cint spacing = buttonSize.y + offsetFromCenter;

    int currentY = rect.h / 2 + spacing * 3;
    nameLabel->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    nameTextBox->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    ipAddressLabel->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    ipAddressTextBox->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    doneButton->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
}

void serverEditor::keyPress(cvk &key) {
    if (key == (vk) keyID::escape) {
        parent->switchVisibleChild(currentServerSelector);
    }
}

void serverEditor::doneButtonMouseDown(const controlEventArgs & /*args*/) {
    serverData data = serverData();
    data.serverIPAddress = sf::IpAddress(WStringToString(ipAddressTextBox->text));
    data.serverName = nameTextBox->text;
    currentServerSelector->servers.push_back(data);
    currentServerSelector->serialize(true);
    currentServerSelector->refresh();
    parent->switchVisibleChild(currentServerSelector);
}
