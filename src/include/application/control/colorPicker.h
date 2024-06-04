#pragma once
#include "control.h"
#include "slider.h"
#include "textBox.h"
#pragma once
struct colorPicker :public control
{
	color currentColor = colorPalette::black;
	slider* channelSliders[bgraColorChannelCount]{};
	textBox* channelBoxes[bgraColorChannelCount]{};
	control* colorPlane = new control();
	slider* sideSlider = new slider(0, 255, 0);

	void setColor(const color& newColor);

	colorPicker();
	virtual void layout(crectanglei2& newRect) override;
};