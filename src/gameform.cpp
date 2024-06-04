#include "gameForm.h"
#include "cpuUsageID.h"
#include "idConverter.h"
#include "gameForm.h"
#include "tag.h"
#include "furnaceRecipe.h"
#include "worldSelector.h"
#include "worldCreator.h"
#include "mainMenu.h"
#include "jigsawPool.h"
#include "settings.h"
#include "resourcePack.h"
#include "benchMark.h"
#include "musicManager.h"
#include "musicList.h"
#include "worldCreator.h"
#include "serverSelector.h"
#include <random>
#include <vector>
#include "constants.h"
#include "application/application.h"
#include "application/control/control.h"
#include "application/control/form/form.h"
#include "globalFunctions.h"
#include "math/graphics/texture.h"
#include "math/random/random.h"
#include "math/rectangle/rectangletn.h"
#include "math/timemath.h"
#include "math/vector/vectn.h"
#include "optimization/benchmark.h"
#include "recipe.h"
#include "structure.h"
#include "serverEditor.h"
#include "client.h"
#include "accountEditor.h"
#include <numeric>
#include <server.h>
#include "sound/soundHandler2D.h"

application *currentApplication = nullptr;

client *currentClient = nullptr;
worldSelector *currentWorldSelector = nullptr;
serverSelector *currentServerSelector = nullptr;
serverEditor *currentServerEditor = nullptr;
gameForm *mainForm = nullptr;
worldCreator *currentWorldCreator = nullptr;
mainMenu *currentMainMenu = nullptr;
accountEditor *currentAccountEditor = nullptr;

// soundHandler2d* handler;

constexpr int normalSmeltingTime = 10 * ticksPerRealLifeSecond;

world *currentWorld = nullptr;

std::mt19937 worldRandom;
std::mt19937 currentRandom;

gameForm::~gameForm()
{
	server::closeAllPorts();
	for (size_t i = 0; i < craftingRecipes.size(); i++)
	{
		delete craftingRecipes[i];
	}

	for (size_t i = 0; i < furnaceRecipes.size(); i++)
	{
		delete furnaceRecipes[i];
	}

	for (size_t i = 0; i < tagList.size; i++)
	{
		delete tagList[i];
	}

	for (size_t i = 0; i < jigsawPoolList.size(); i++)
	{
		delete jigsawPoolList[i];
	}

	for (size_t i = 0; i < converterList.size(); i++)
	{
		delete converterList[i];
	}

	for (size_t i = 0; i < structureList.size(); i++)
	{
		delete structureList[i];
	}
	unloadResourcePacks();
}

bool gameForm::close()
{
	if (currentServer)
	{
		currentServer->stop();
	}
	return true;
	// can't close if you're not on the main menu
	// return currentMainMenu->visible;
}

void gameForm::layout(crectanglei2 &rect)
{
	form::layout(rect);
	for (control *c : children)
	{
		c->layout(rect);
	}
	// currentMainMenu->layout(rect);
	// currentWorldSelector->layout(rect);
	// currentWorldCreator->layout(rect);
	// currentServerSelector->layout(rect);
	// currentServerEditor->layout(rect);
	// currentClient->layout(rect);
	// currentAccountEditor->layout(rect);
}

gameForm::gameForm() : form()
{

	currentRandom = getRandomFromSeed(getmicroseconds());

	// handler = new soundHandler2d();

	loadResourcePacks();

	// load controls
	mainForm = this;

	currentMainMenu = new mainMenu();

	currentClient = new client();

	currentAccountEditor = new accountEditor(); // initialize accounteditor after the client, as the account editor edits data which the client has to retrieve first

	currentWorldSelector = new worldSelector();
	currentWorldSelector->refresh(); // can't put refresh() in base constructor because refresh() calls a virtual function
	currentWorldCreator = new worldCreator();

	currentServerSelector = new serverSelector();
	currentServerSelector->refresh(); // can't put refresh() in base constructor because refresh() calls a virtual function
	currentServerEditor = new serverEditor();

	addChildren({currentMainMenu, currentAccountEditor,
				 currentWorldSelector, currentWorldCreator,
				 currentServerSelector, currentServerEditor,
				 currentClient});
	for (auto *&c : children)
	{
		c->visible = false;
	}
	currentMainMenu->visible = true;
	focusChild(currentMainMenu);
}

void gameForm::render(cveci2 &position, const texture &renderTarget)
{
	// the game will update music for itself, depending on the situation
	if (!currentClient->visible)
	{
		updateMusic(mainMenuBackgroundMusic.get());
	}
	handler.hearingRange = currentClient->hearingRange2D;
	handler.earPosition = currentClient->earPosition;
	handler.earSpeed = currentClient->earSpeed;
	handler.globalVolume = settings::soundSettings::volume;
	handler.update();

	currentApplication->window->setMouseCursorVisible(focusedChild != currentClient);

	// draw all controls
	renderChildren(position, renderTarget);
	// don't draw self
	// form::render(position, renderTarget);
	//handler.visualize(renderTarget);
}