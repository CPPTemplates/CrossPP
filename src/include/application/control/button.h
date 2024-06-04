#include "control.h"
#include "math/timemath.h"
#pragma once
struct button :public control 
{
	button(const std::wstring& text = std::wstring());
	miliseconds lastClickTime = 0;
	color clickColor = colorPalette::white;
	virtual void click() override;
	virtual void render(cveci2& position, const texture& renderTarget) override;
};