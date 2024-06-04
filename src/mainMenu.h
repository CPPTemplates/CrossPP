#pragma once
#include "application/control/form/form.h"
#include "application/control/button.h"
#include "application/control/pictureBox.h"
#include "application/control/label.h"
struct mainMenu : public form
{
	mainMenu();
	button* playOfflineButton = new button(L"Play Offline");
	button* playOnlineButton = new button(L"Play Online");
	button* accountButton = new button(L"Account");
	pictureBox* backgroundPicture = nullptr;
	label* gameNameLabel = nullptr;
	virtual void render(cveci2& position, const texture& renderTarget) override;
	virtual void layout(crectanglei2& newRect) override;
	virtual void focus() override;
	virtual void keyDown(cvk& key) override;
    void clickedOnButton(const controlEventArgs& args);
};
extern mainMenu* currentMainMenu;