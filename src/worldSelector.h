#pragma once
#include "gameSelector.h"

struct worldSelector : gameSelector
{
	//show a list with save files
	worldSelector();
	virtual void addGame() override;
	virtual void addGameControls() override;
	virtual void openGame(const control* const c) const override;
	virtual void deleteGame(const control* const c) override;
};
extern worldSelector* currentWorldSelector;