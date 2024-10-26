#include "worldSelector.h"
#include "world.h"
#include "saveFileControl.h"
#include "gameForm.h"
#include "filesystem/filemanager.h"
#include "folderList.h"
#include "worldCreator.h"
#include "server.h"
#include "client.h"
#include <filesystem>
#include <string>
#include "gameSelector.h"
#include "application/control/control.h"
worldSelector::worldSelector() : gameSelector()
{
	openButton->text = std::wstring(L"Open Selected World");
	newButton->text = std::wstring(L"Create New World");
	gameTypeName = L"world";
}

void worldSelector::addGame()
{
	mainForm->switchVisibleChild(currentWorldCreator);
}

void worldSelector::addGameControls()
{
    std::set<stdFileSystem::path> sortedByName;
	//generate save folder controls
    for (const auto &entry : stdFileSystem::directory_iterator(savesFolder))
    {
        sortedByName.insert(entry.path());
    }

	for (const auto& folderIterator : sortedByName)
	{
		gameListControl->children.push_back(new saveFileControl(folderIterator.filename().wstring()));
	}
}

void worldSelector::openGame(const control* const c) const
{
	currentWorld = new world();
	currentWorld->name = ((saveFileControl*)c)->gameName;
	if (currentWorld->serialize(false))
	{
		createServerFromCurrentWorld();
		mainForm->switchVisibleChild(currentClient);
	}
}

void worldSelector::deleteGame(const control* const c)
{
	//delete folder
	const std::wstring worldToRemoveName = ((saveFileControl*)c)->gameName;
	std::filesystem::remove_all(savesFolder / worldToRemoveName );
}