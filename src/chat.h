#pragma once
#include <vector>
#include <string>
#include "array/wstringFunctions.h"
#include "dimensionID.h"
//#include "gameControl.h"
struct chat
{

	//will not be saved when saving the world
	std::vector<std::wstring> linesSaid = std::vector<std::wstring>();
	std::vector<int> timesSaid = std::vector<int>();

	bool getLocation(const human& sender, const wstringContainer& container, int& offset, dimensionID& dimensionIn, vec2& position);
	bool getBlockToUse(const human& sender, const wstringContainer& container, int& offset, blockID& result);
	bool selectEntities(const human& sender, const wstringContainer& container, int& offset, std::vector<entity*>& result);
	virtual void say(human& sender, std::wstring line);
	virtual void addLine(const std::wstring& line);
	virtual void render(const texture& renderTarget, const gameControl& screen) const;
};