#pragma once
#include "facingData.h"
struct jigsawData :facingData
{
	//other structures will connect to this
	std::wstring jigsawName = std::wstring();
	//connect to jigsaw blocks with this name
	std::wstring targetName = std::wstring();

	std::wstring targetPoolName = std::wstring();
	//whether the placed structure can be rolled in order to fit
	bool rollable = false;

	//if the structure this jigsaw places keeps it jigsaws or generates the rest of the structure
	bool keepJigsaws = false;

	blockID turnsInto = blockID::air;

	int levels = 0;
	virtual void serializeValue(nbtSerializer& s) override;
};