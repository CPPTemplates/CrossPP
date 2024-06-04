#pragma once
#include "application/control/form/form.h"
#include "application/control/button.h"
#include "application/control/slider.h"
struct videoSettingsForm :form
{
	button* graphicsModeButton = new button();

	button* transparencyButton = new button();
	button* showShadowsButton = new button();
	button* biomeColorsButton = new button();

	button* fullScreenButton = new button();
	slider* guiScaleSlider = new slider(0.5, 2, 1);

	videoSettingsForm();
	virtual void render(cveci2& position, const texture& renderTarget) override;
	virtual void layout(crectanglei2& newRect) override;
	virtual void mouseDown(cveci2& position, cmb& button) override;
};