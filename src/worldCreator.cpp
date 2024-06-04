#include "worldCreator.h"
#include "world.h"
#include "gameControl.h"
#include "gameForm.h"
#include "keyID.h"
#include "worldSelector.h"
#include "folderList.h"
#include "client.h"
#include "server.h"

worldCreator::worldCreator() : form() {
    worldCreating = new world();
    addChildren({worldNameLabel, worldNameTextBox,
                 willBeSavedInLabel,
                 worldSeedLabel, worldSeedTextBox,
                 cheatsButton,
                 createButton,
                 cancelButton});
    addEventHandlers(&worldCreator::onClick, createButton->onClick, cheatsButton->onClick,
                     cancelButton->onClick);
}

void worldCreator::onClick(const controlEventArgs &args) {
    if (&args.sender == createButton) {
        worldCreating->name = getAvailableWorldName(worldNameTextBox->text);
        // create a new world
        if (worldSeedTextBox->text.length()) {
            long seed = 0;

            if (convertToLong(worldSeedTextBox->text, seed)) {
                worldCreating->seed = seed;
            } else {
                worldCreating->seed = std::hash<std::wstring>()(worldSeedTextBox->text);
            }
            worldSeedTextBox->text = std::wstring();
        } else {
            worldCreating->seed = rand<ull>(currentRandom);
            // Create a random number generator engine
            //std::random_device rd;       // Seed for the random number generator
            //std::mt19937_64 gen(rd()); // Mersenne Twister 64-bit engine
//
            //// Define the range for the random numbers
            //std::uniform_int_distribution<unsigned long long> dis(
            //        std::numeric_limits<unsigned long long>::min(),
            //        std::numeric_limits<unsigned long long>::max());
//
            //// Generate a random unsigned long long
            //unsigned long long random_number = dis(gen);


            //worldCreating->seed =
        }
        //} else {
        //    worldCreating->seed = getmicroseconds();
        //}
        worldNameTextBox->text = std::wstring();
        //'move' the new world to the game
        currentWorld = worldCreating;
        worldCreating = new world();
        generateNewWorld();

        createServerFromCurrentWorld();


        mainForm->switchVisibleChild(currentClient);
    } else if (&args.sender == cheatsButton) {
        worldCreating->
                allowCheats = !worldCreating->allowCheats;
    } else if (&args.sender == cancelButton) {
        cancelWorldCreation();

    }
}

void worldCreator::keyPress(cvk &key) {
    if (key == (vk) keyID::escape) {
        cancelWorldCreation();
    }
}

void worldCreator::layout(crectanglei2 &newRect) {
    form::layout(newRect);
    // center controls
    cint offsetFromCenter = 0x3;
    cint middleX = (rect.w - buttonSize.x) / 2;
    cint leftSideX = rect.w / 2 - buttonSize.x - offsetFromCenter;
    cint rightSideX = rect.w / 2 + offsetFromCenter;
    cint spacing = buttonSize.y + offsetFromCenter;

    int currentY = rect.h / 2 + spacing * 3;
    worldNameLabel->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    worldNameTextBox->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    willBeSavedInLabel->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    worldSeedLabel->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    worldSeedTextBox->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    cheatsButton->layout(crectanglei2(middleX, currentY, buttonSize.x, buttonSize.y));
    currentY -= spacing;
    createButton->layout(crectanglei2(leftSideX, currentY, buttonSize.x, buttonSize.y));
    cancelButton->layout(crectanglei2(rightSideX, currentY, buttonSize.x, buttonSize.y));
}

void worldCreator::render(cveci2 &position, const texture &renderTarget) {
    cheatsButton->text = std::wstring(L"Allow Cheating: ") +
                         (worldCreating->allowCheats ? std::wstring(L"On") : std::wstring(L"Off"));
    willBeSavedInLabel->text =
            std::wstring(L"Will Be Saved In: ") + getAvailableWorldName(worldNameTextBox->text);
    renderOptionsBackGround(crectanglei2(position, rect.size), renderTarget);
    renderChildren(position, renderTarget);
}

void worldCreator::cancelWorldCreation() {
    visible = false;
    currentWorldSelector->visible = true;
    mainForm->focusChild(currentWorldSelector);
}

worldCreator::~worldCreator() {
    delete worldCreating;
}

std::wstring getAvailableWorldName(std::wstring worldName) {

    if (worldName.length() == 0) {
        worldName = std::wstring(L"World");
    } else {
        std::wstring newName = std::wstring();
        bool addedUnderScore = false;
        for (size_t i = 0; i < worldName.length(); i++) {
            std::wstring allowedChars = std::wstring(
                    L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-() ");
            std::wstring allowedCharsAfterStart = std::wstring(L"01234556789");
            if ((allowedChars.find(worldName[i]) != std::wstring::npos) ||
                (i && (allowedCharsAfterStart.find(worldName[i]) != std::wstring::npos))) {
                newName.push_back(worldName[i]);
                addedUnderScore = false;
            } else {
                if (!addedUnderScore) {
                    addedUnderScore = true;
                    newName.push_back(L'_');
                }
            }
        }
        worldName = newName;
    }

    if (stdFileSystem::exists(savesFolder / worldName)) {
        int index = 1;
        while (true) {
            const std::wstring &currentWorldName =
                    worldName + std::wstring(L"(") + std::to_wstring(index) + std::wstring(L")");
            if (stdFileSystem::exists(savesFolder / currentWorldName)) {
                index++;
            } else {
                return currentWorldName;
            }
        }
    } else {
        return worldName;
    }
}
