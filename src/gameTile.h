#pragma once

#include "application/control/form/form.h"
#include "application/control/pictureBox.h"
#include "application/control/label.h"
struct gameTile : form
{
	gameTile(const std::wstring& saveFileName);
	pictureBox* screenShot = nullptr;
	label* infoLabel;
	std::wstring gameName = std::wstring();
	virtual ~gameTile() override;
	virtual void layout(crectanglei2& newRect) override;
};