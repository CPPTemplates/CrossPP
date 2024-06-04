#pragma once
#include "slider.h"
#pragma once
struct listControl :public control
{
	veci2 childSize = veci2();
	slider* sideSlider = new slider(0, 1, 1);
	int sideSliderWidth = 0;
	listControl(cint& sideSliderWidth = 0x10);
	virtual void reOrganizeChildControls();
    control* selectedChild = nullptr;
    void mouseDown(cveci2& position, cmb& button) override;
	virtual void layout(crectanglei2& newRect) override;
    void sideSliderOnValueChanged(const valueEventArgs& args);
};