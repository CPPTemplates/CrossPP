#pragma once
#include "application/control/form/form.h"
#include "application/control/label.h"
#include "application/control/slider.h"
#include "settings.h"
struct soundSettingsForm : form
{
	label* volumeLabel = new label();
	slider* volumeSlider = new slider(0, 1, 1);
	label* headScreenDistanceLabel = new label();
	slider* headScreenDistanceSlider = new slider(0, 5, settings::soundSettings::headScreenDistance);
	button* musicButton = new button();
	soundSettingsForm();
	virtual void layout(crectanglei2& newRect) override;
	virtual void render(cveci2& position, const texture& renderTarget) override;
	virtual void mouseDown(cveci2& position, cmb& button) override;
	virtual void keyDown(cvk& keycode) override;
};