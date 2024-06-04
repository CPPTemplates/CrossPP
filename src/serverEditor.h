#pragma once
#include "application/control/form/form.h"
#include <application/control/textBox.h>
#include <application/control/button.h>
#include <application/control/label.h>
struct serverEditor :form {
	serverEditor();
	label* nameLabel = new label(L"Name...");
	textBox* nameTextBox = new textBox();
	label* ipAddressLabel = new label(L"ipaddress:port");
	textBox* ipAddressTextBox = new textBox();
	button* doneButton = new button(L"Done");
	virtual void layout(crectanglei2& newRect) override;
	virtual void keyPress(cvk& key) override;
    void doneButtonMouseDown(const controlEventArgs& args);
};
extern serverEditor* currentServerEditor;