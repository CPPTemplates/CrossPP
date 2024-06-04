#include "colorPicker.h"
colorPicker::colorPicker()
{
	children.push_back(colorPlane);
	children.push_back(sideSlider);
	for (fsize_t channel = 0; channel < bgraColorChannelCount; channel++)
	{
		channelSliders[channel] = new slider(0, 255, 0);
		channelBoxes[channel] = new textBox();
		children.push_back(channelSliders[channel]);
		children.push_back(channelBoxes[channel]);
	}
	children.update();
}

void colorPicker::layout(crectanglei2& newRect)
{
	int colorPlaneWidth = newRect.w * 4 / 10;
	colorPlane->layout(crectanglei2(newRect.x, newRect.y, colorPlaneWidth, newRect.h));

	int colorPlaneAndSideSliderWidth = newRect.w / 2;
	int sideSliderWidth = colorPlaneAndSideSliderWidth - colorPlaneWidth;
	sideSlider->layout(crectanglei2(newRect.x + colorPlaneWidth, newRect.y, sideSliderWidth, newRect.h));

	int channelSliderX = newRect.x + colorPlaneAndSideSliderWidth;
	int channelTextboxX = newRect.x + newRect.w * 9 / 10;

	int channelSliderHeight = newRect.h / 4;


	for (int channel = 0; channel < (int)bgraColorChannelCount; channel++)
	{
		channelSliders[channel]->layout(crectanglei2(channelSliderX, newRect.y + newRect.h - channelSliderHeight * (channel + 1), channelTextboxX - channelSliderX, channelSliderHeight));
		channelBoxes[channel]->layout(crectanglei2(channelTextboxX, newRect.y + newRect.h - channelSliderHeight * (channel + 1), (newRect.x + newRect.w) - channelTextboxX, channelSliderHeight));
	}
}
void colorPicker::setColor(const color& newColor)
{
	if (newColor != currentColor)
	{
		for (fsize_t channel = 0; channel < bgraColorChannelCount; ++channel)
		{
			channelSliders[channel]->setValue(newColor[channel]);
		}
		currentColor = newColor;
	}
}