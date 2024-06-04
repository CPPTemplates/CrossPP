#pragma once
#include "gameTile.h"
#include "application/control/pictureBox.h"
#include "application/control/label.h"
struct saveFileControl : gameTile
{
	saveFileControl(const std::wstring& saveFileName);
};