#pragma once

#include "application/control/form/form.h"
#include "structureBlockUI.h"
#include "jigsawUI.h"
#include "inventoryForm.h"
#include "settingsForm.h"
#include "videoSettingsForm.h"
#include "soundSettingsForm.h"
#include "musicCollection.h"
#include "gameRenderData.h"
#include "network/client/clientInput.h"
#include "creditsForm.h"
#include "soundPacket.h"
#include "application/control/touchJoystick.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <application/control/pictureBox.h>

constexpr fp defaultGuiTextureSize = (fp) 0x200;
struct playerSocket;

//the game will be rendered on a texture using this class
struct gameControl : form, clientInput {
    gameControl(playerSocket &socket);

    seconds currentFrameStartSeconds = 0;

    //controls
    settingsForm *options = new settingsForm();
    videoSettingsForm *videoOptions = new videoSettingsForm();
    soundSettingsForm *soundOptions = new soundSettingsForm();
    structureBlockUI *structureBlockOptions = new structureBlockUI();
    jigsawUI *jigsawOptions = new jigsawUI();
    textBox *commandLineTextbox = new textBox(false);
    inventoryForm *inventoryUI = new inventoryForm();
    creditsForm *currentCredits = new creditsForm();

    struct eventTranslator* translator;

    bool startCredits = false;
    //touch variables
    bool touchInput = false;

    bool touchStarted = false;
    bool touching = false;
    bool touchEnded = false;

    //connections
    playerSocket &socket;
    human *player;//for less code

    bool wantsClipboardInput = false;
    std::wstring copyToClipboard = std::wstring();

    //rendering
    //only use the camera position for rendering!
    vec2 cameraPosition = vec2();

    vec2 visibleRange = vec2(defaultVisibleRangeXWalk);

    veci2 unFocusedMousePosition = veci2();
    //veci2 mousePositionPixels = veci2();

    //veci2 newMousePositionPixels = veci2();
    //controls respond faster, so we have another input for controls

    clientInput mostRecentInput = clientInput();

    bool clickedFocused[(byte) sf::Mouse::ButtonCount];

    //creditsForm* currentCredits = nullptr;

    mat3x3 worldToRenderTargetTransform = mat3x3();

    std::vector<soundPacket> dataToSend = std::vector<soundPacket>();
    touchJoystick *moveJoystick = nullptr;
    touchJoystick *interactJoystick = nullptr;
    pictureBox* chatButton = nullptr;
    pictureBox* settingsButton = nullptr;
    pictureBox* inventoryButton = nullptr;

    inline fp getHUDScale() const{
        return (settings::videoSettings::guiScale * rect.size.x) /
               ((StandardInventoryColumnCount * 2) * hotbarSpacing);
    }
    //overrides
    //virtual void render(cveci2& position, const texture& renderTarget) override;
    void render(cveci2 &position, const texture &renderTarget) override;

    virtual void layout(crectanglei2 &newRect) override;

    virtual void focus() override;

    virtual void lostFocus() override;

    void onJoystickTouch(const mouseButtonEventArgs &args);
    void onJoystickTouchEnd(const dragEventArgs& args);
    void onButtonClick(const controlEventArgs &args);
    void switchInventoryGUI();

    bool processScreenTouch(const dragEventArgs &args);

    void addTouchInput();

    void processInput();

    void updateTransforms(cvec2& headPosition);

    void renderGame(crectanglei2 &rect, const texture &renderTarget, cbool &renderHUD);

    void commandLineKeyPressed(const keyEventArgs& e);

    void serializeMusicPreference(nbtSerializer& serializer);

    gameRenderData getRenderData(const texture &renderTarget, cfp &secondsOffset = 0);

    mat3x3 getWorldToScreenTransform(cvec2 &middleWorldPosition, cfp &pixelsPerBlock);
    bool getWorldFocus() const;
};

extern std::mt19937 currentRandom;

void renderIcons(const std::vector<fp> &values, const std::vector<rectangle2> &iconFullTextureRects,
                 const std::vector<rectangle2> &iconHalfTextureRects, crectangle2 &firstIconRect,
                 cfp &xOffset, const texture &renderTarget);

void renderOptionsBackGround(crectanglei2 &rect, const texture &renderTarget);