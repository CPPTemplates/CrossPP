#pragma once
#include "application/control/slider.h"
#include "application/control/button.h"
#include "application/control/textBox.h"
#include "application/control/label.h"
#include "constants.h"
#include "application/control/form/form.h"
struct jigsawUI : form
{
	jigsawData* temporaryJigsawData = nullptr;
	label* targetPoolLabel = new label();
	textBox* targetPoolTextbox = new textBox();
	label* nameLabel = new label();
	textBox* nameTextbox = new textBox();
	label* targetNameLabel = new label();
	textBox* targetNameTextbox = new textBox();
	label* turnsIntoLabel = new label();
	textBox* turnsIntoTextbox = new textBox();
	label* jointTypeLabel = new label();
	button* jointTypeButton = new button();
	slider* levelsSlider = new slider(0, maxJigsawLevels, 0);
	button* keepJigsawsButton = new button();
	button* generateButton = new button();
	button* doneButton = new button();
	button* cancelButton = new button();
	jigsawUI();
	void serializeWithTemporaryData(cbool& write);
	virtual void layout(crectanglei2& rect) override;
	virtual void render(cveci2& position, const texture& renderTarget) override;
	virtual void mouseDown(cveci2& position, cmb& button) override;
};