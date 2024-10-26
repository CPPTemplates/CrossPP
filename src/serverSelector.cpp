#include "serverSelector.h"
#include "world.h"
#include "saveFileControl.h"
#include "gameControl.h"
#include "gameForm.h"
#include "filesystem/filemanager.h"
#include "folderList.h"
#include "serverEditor.h"
#include "serverControl.h"
#include "client.h"
#include "array/arrayFunctions/arrayFunctions.h"

serverSelector::serverSelector() : gameSelector()
{
	gameTypeName = L"server";
	this->openButton->text = L"Join Server";
	this->newButton->text = L"Add Server...";
}

void serverSelector::addGame()
{
	mainForm->switchVisibleChild(currentServerEditor);
}

void serverSelector::addGameControls()
{
	serialize(false);


	//generate save folder controls
	for (const auto& server : servers)
	{
		gameListControl->children.push_back(new serverControl(server));
	}
}

void serverSelector::openGame(const control* const c) const
{
	if (currentClient->connectToServer(((serverControl*)c)->linkedData)) {
		mainForm->switchVisibleChild(currentClient);
	}
}

void serverSelector::deleteGame(const control* const c)
{
	servers.erase(find(servers, ((serverControl*)c)->linkedData));
	serialize(true);
}