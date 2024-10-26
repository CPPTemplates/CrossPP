#pragma once
#include "application/control/slider.h"
#include "application/control/button.h"
#include "application/control/textBox.h"
#include "application/control/label.h"
#include "structureBlockModeUI.h"

struct structureBlockLoadModeUI : structureBlockModeUI
{
	label* relativePositionLabel = new label();
	label* showBoundingBoxLabel = new label();

	textBox* relativeXTextbox = new textBox();
	textBox* relativeYTextbox = new textBox();
	button* showBoundingBoxButton = new button();

	label* integrityAndSeedLabel = new label();
	slider* integritySlider = new slider(0, 1, 1.0);
	textBox* seedTextbox = new textBox();
	label* includeEntitiesLabel = new label();

	button* includeEntitiesButton = new button();
	button* replaceBlocksButton = new button();
	button* mirrorYButton = new button();
	slider* rotationSlider = new slider(0, 270, 0);
	button* loadButton = new button();
	structureBlockLoadModeUI();
	void serializeWithTemporaryData(cbool& write);
	virtual void layout(crectanglei2& newRect) override;
	virtual void mouseDown(cveci2& position, cmb& button) override;
	virtual void render(cveci2& position, const texture& renderTarget) override;
};