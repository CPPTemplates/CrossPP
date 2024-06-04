#pragma once
#include "gameSelector.h"
#include <vector>
#include "serverData.h"
#include "serverDataList.h"

struct serverSelector : gameSelector, serverDataList
{
	serverSelector();
	virtual void addGame() override;
	virtual void addGameControls() override;
	virtual void openGame(const control* const c) const override;
	virtual void deleteGame(const control* const c) override;
};
extern serverSelector* currentServerSelector;