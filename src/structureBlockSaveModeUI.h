#pragma once
#include "application/control/button.h"
#include "structureBlockModeUI.h"
#include "application/control/label.h"
#include "application/control/textBox.h"
struct structureBlockSaveModeUI : structureBlockModeUI
{
	label* relativePositionLabel = new label();
	label* showInvisibleBlocksLabel = new label();

	textBox* relativeXTextbox = new textBox();
	textBox* relativeYTextbox = new textBox();
	button* showInvisibleBlocksButton = new button();

	label* sizeLabel = new label();
	label* detectLabel = new label();

	textBox* sizeXTextbox = new textBox();
	textBox* sizeYTextbox = new textBox();
	button* detectButton = new button();

	label* includeEntitiesLabel = new label();

	button* includeEntitiesButton = new button();

	button* saveButton = new button();
	structureBlockSaveModeUI();
	void serializeWithTemporaryData(cbool& write);
	virtual void layout(crectanglei2& newRect) override;
	virtual void mouseDown(cveci2& position, cmb& button) override;
	virtual void render(cveci2& position, const texture& renderTarget) override;
};