#pragma once
#include "application/control/form/form.h"
#include "application/control/button.h"
struct settingsForm :form
{
	button* saveAndQuitButton = new button(L"Save And Quit To Title");
	button* videoSettingsButton = new button(L"Video Settings...");
	button* soundSettingsButton = new button(L"Sound Settings...");
	settingsForm();
	virtual void render(cveci2& position, const texture& renderTarget) override;
	virtual void mouseDown(cveci2& position, cmb& button) override;
	virtual void layout(crectanglei2& newRect) override;
};