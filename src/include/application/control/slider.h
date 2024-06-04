#include "control.h"
#include "button.h"
#pragma once

struct valueEventArgs : controlEventArgs{
    fp value = 0;
    valueEventArgs(control& sender, cfp& value) : controlEventArgs(sender), value(value){}
};

typedef eventHandler<valueEventArgs> valueEventHandler;

struct slider :public control
{
	slider(cfp& minValue, cfp& maxValue, cfp& value, cfp& step = 1);
	button* decreaseButton = new button();
	button* increaseButton = new button();
	button* dragButton = new button();

    valueEventHandler onValueChanged = valueEventHandler();

	fp value = 0;
	fp minValue, maxValue;
	fp step = 1;

	//clamped
	void rePositionDragButton();
	void setValue(cfp& newValue);

	void onClick(const controlEventArgs& args);
	virtual void dragCompleted();
	virtual void layout(crectanglei2& newRect) override;
	color checkMarkColor = colorPalette::white;

    void mouseDown(cveci2 &position, cmb &button) override;
};