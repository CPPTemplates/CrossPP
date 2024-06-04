#include "application/control/form/form.h"
#include "application/control/button.h"
#include "application/control/textBox.h"
#include "math/rectangle/rectangletn.h"
#pragma once

bool checkAccountName(const std::wstring& n);

struct accountEditor : form {
	textBox* playerNameTextBox = new textBox();
	button* saveButton = new button(L"Save");

	accountEditor();
	virtual void layout(crectanglei2& newRect) override;
	virtual void mouseDown(cveci2& position, cmb& button) override;
	virtual void renderBackGround(cveci2& position, const texture& renderTarget) override;
};
extern accountEditor* currentAccountEditor;