#include "button.h"
#include "math/graphics/brush/brushes.h"
#include "math/graphics/color/colorFunctions.h"

button::button(const std::wstring& text) :control()
{
	this->text = text;
}

void button::render(cveci2& position, const texture& renderTarget)
{
	miliseconds currentTime = getMiliseconds();
	fp part = (fp)(currentTime - lastClickTime) / 500.0;//500ms effect

	color c;
	if (part < 1) 
	{
		c = lerpColor(clickColor, backGroundColor, part);
	}
	else 
	{
		c = backGroundColor;
	}
	fillRectangle(renderTarget, rectanglei2(position + borderSize, rect.size - (borderSize + borderSize)), solidColorBrush(c));

	renderBorder(position, renderTarget);
	renderText(position, renderTarget);
}

void button::click()
{
	lastClickTime = getMiliseconds();
	control::click();
}
