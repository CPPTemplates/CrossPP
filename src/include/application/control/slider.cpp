#include "slider.h"

slider::slider(cfp &minValue, cfp &maxValue, cfp &value, cfp &step) : control(),
																	  value(value), minValue(minValue), maxValue(maxValue), step(step)
{
	addChildren({increaseButton, decreaseButton, dragButton});
}

void slider::rePositionDragButton()
{
	// calculate the x position of the slider
	cfp &part = math::getw(minValue, maxValue, value);

	cint &horizontalAxis = rect.w > rect.h ? 0 : 1;
	cint &room = rect.size.axis[horizontalAxis] - decreaseButton->rect.size.axis[horizontalAxis] - dragButton->rect.size.axis[horizontalAxis] - increaseButton->rect.size.axis[horizontalAxis];
	dragButton->rect.pos0.axis[horizontalAxis] = (decreaseButton->rect.size[horizontalAxis]) + (int)(part * room);
}

void slider::setValue(cfp &newValue)
{
	cfp &correctedValue = math::clamp(newValue, minValue, maxValue);
	if (value != correctedValue)
	{
		value = correctedValue;
		rePositionDragButton();
		onValueChanged.invoke(valueEventArgs(*this, correctedValue));
	}
}

void slider::mouseDown(cveci2 &position, cmb &button)
{
	if (isNormalClick(button))
	{
		control *child = getHighestChild(position);
		if (!child || child == dragButton)
		{
			cint horizontalAxis = rect.w > rect.h ? 0 : 1;

			// the background was clicked
			cint newDragButtonX = position.axis[horizontalAxis] - dragButton->rect.size.axis[horizontalAxis] / 2;
			dragButton->rect.pos0.axis[horizontalAxis] = math::clamp(newDragButtonX, increaseButton->rect.size.axis[horizontalAxis], rect.size.axis[horizontalAxis] - increaseButton->rect.size.axis[horizontalAxis] - dragButton->rect.size.axis[horizontalAxis]);
			;
			dragCompleted();
		}
		else
		{
			if (child == decreaseButton)
			{
				setValue(value - step);
			}
			else if (child == increaseButton)
			{
				setValue(value + step);
			}
		}
	}
	control::mouseDown(position, button);
}

void slider::dragCompleted()
{
	// calculate new value
	cint horizontalAxis = rect.w > rect.h ? 0 : 1;
	cfp part = dragButton->rect.pos0.axis[horizontalAxis] - decreaseButton->rect.size.axis[horizontalAxis];
	cfp max = increaseButton->rect.pos0.axis[horizontalAxis] - dragButton->rect.size.axis[horizontalAxis] - decreaseButton->rect.size.axis[horizontalAxis];
	setValue(math::mapValue(part, (fp)0, max, minValue, maxValue));
}

void slider::layout(crectanglei2 &newRect)
{
	control::layout(newRect);
	if (rect.w > rect.h) // horizontal
	{
		cint buttonWidth = (int)(rect.h / math::goldenRatio);
		increaseButton->layout(rectanglei2(rect.w - buttonWidth, 0, buttonWidth, rect.h));
		decreaseButton->layout(rectanglei2(0, 0, buttonWidth, rect.h));
		dragButton->layout(rectanglei2(0, 0, buttonWidth, rect.h));
	}
	else
	{
		cint buttonHeight = (int)(rect.w / math::goldenRatio);
		increaseButton->layout(rectanglei2(0, rect.h - buttonHeight, rect.w, buttonHeight));
		decreaseButton->layout(rectanglei2(0, 0, rect.w, buttonHeight));
		dragButton->layout(rectanglei2(0, 0, rect.w, buttonHeight));
	}

	rePositionDragButton();
}

void slider::onClick(const controlEventArgs &args)
{
}
