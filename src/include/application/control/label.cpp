#include "label.h"
label::label(const std::wstring& text) :control()
{
	borderSize = 0;
	backGroundColor = colorPalette::transparent;
	this->text = text;
}