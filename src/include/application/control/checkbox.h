#include "control.h"
#pragma once
struct checkBox :public control
{
	checkBox();
	void (*onCheckedChanged)() = nullptr;
	bool checked = false;
	virtual void click() override;
	virtual void checkedChanged();
	virtual void render(cveci2& position, const texture& renderTarget) override;
	virtual void drawCheckMark(cveci2& position, const texture& renderTarget);
	color checkMarkColor = colorPalette::white;
};