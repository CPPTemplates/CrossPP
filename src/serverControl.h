#pragma once
#include "gameTile.h"
#include "serverData.h"
struct serverControl : gameTile
{
	//const reference because the control itself cannot modify the data. the editor can
	const serverData& linkedData;
	serverControl(const serverData& linkedData);
};