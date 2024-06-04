#include "checkbox.h"
#include "math/graphics/brush/brushes.h"

checkBox::checkBox() :control()
{

}

void checkBox::click()
{
	checked = !checked;
	checkedChanged();
}

void checkBox::checkedChanged()
{
	if (onCheckedChanged)
	{
		onCheckedChanged();
	}
}

void checkBox::render(cveci2& position, const texture& renderTarget)
{
	control::render(position, renderTarget);
	if (checked)
	{
		//draw check mark on top
		drawCheckMark(position, renderTarget);
	}
}

void checkBox::drawCheckMark(cveci2& position, const texture& renderTarget)
{
	//two lines
	fillLine(renderTarget, (vec2)position + (vec2)rect.size * vec2(0.2, 0.5), (vec2)position + (vec2)rect.size * vec2(0.5, 0.2), solidColorBrush(checkMarkColor));
	fillLine(renderTarget, (vec2)position + (vec2)rect.size * vec2(0.5, 0.2), (vec2)position + (vec2)rect.size * vec2(1.1, 1.1), solidColorBrush(checkMarkColor));
}
