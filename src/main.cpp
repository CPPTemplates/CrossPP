#include "main.h"
// #include <crtdbg.h>
#include "constants.h"
#include "gameForm.h"
#include "application/application.h"
#include "globalFunctions.h"
#include "array/wstringFunctions.h"
#include <thread>
#include <filesystem>
#include <string>
#include "folderList.h"
#include <filesystem/filemanager.h>

#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include "application/thread/setThreadName.h"
#include "include/optimization/stableTickLoop.h"
#include <list>
#include "filesystem/fileFunctions.h"
#include "include/math/random/shufflerandom.h"
#include <bitset>
#include "include/math/rectangle/rectanglefunctions.h"
#include "application/initialize.h"
#include "world.h"
#include "include/optimization/handleError.h"

bool loadingFinished = false;
sf::RenderWindow *window = nullptr;

struct balloon
{
    sf::CircleShape shape;
    sf::Vector2f speed;
};

void showLoadingScreen()
{
    // Create the main rendering window
    window = application::createWindow(gameName);

    sf::Font font;
    // this will also work on android when the data folder has not been copied yet,
    // as it will look for data/font.otf in the assets folder.
    font.loadFromFile("data/font.otf");

    stableLoop loop = stableLoop(1000000 / 60);

    std::list<balloon> shapes = std::list<balloon>();

    // Start game loop
    while (window->isOpen())
    {
        loop.waitTillNextLoopTime();
        // Process events
        sf::Event Event;
        while (window->pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                window->close();
            else if (Event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.0f, 0.0f, (float)Event.size.width, (float)Event.size.height);
                window->setView(sf::View(visibleArea));
                // Clear the screen (fill it with black color)
                shapes = std::list<balloon>();
            }
        }
        if (loadingFinished)
        {
            return;
        }

        window->clear(sf::Color::Magenta);
        auto size = window->getView().getSize();
        const auto &minSize = math::minimum(window->getView().getSize().y, window->getView().getSize().x);

        if (shapes.size() < 0x1000)
        {
            balloon newBalloon;
            auto pos = getrandomPosition(currentRandom, rectangle2(vec2(size.x, size.y)));
            newBalloon.shape.setPosition(sf::Vector2f((float)pos.x, (float)pos.y));
            newBalloon.shape.setFillColor(sf::Color(rand(currentRandom, bytemax), rand(currentRandom, bytemax), rand(currentRandom, bytemax)));
            newBalloon.shape.setRadius(1); // it should start as a single pixel
            shapes.push_front(newBalloon);
        }
        for (auto it = shapes.begin(); it != shapes.end();)
        {
            auto &balloon = *it;
            const auto &pos = balloon.shape.getPosition();
            const auto &radius = balloon.shape.getRadius();
            const auto &diameter = radius + radius;
            if (pos.x < -diameter || pos.y < -diameter || pos.x > size.x || pos.y > size.y)
            {
                it = shapes.erase(it);
            }
            else
            {
                const auto &variance = radius / 30;
                balloon.speed += sf::Vector2f(randFp(currentRandom, -variance, variance), randFp(currentRandom, -variance, variance));
                balloon.speed *= 0.95f;
                balloon.shape.setPosition(pos + balloon.speed);
                balloon.shape.setRadius(radius * 1.001f);
                window->draw(balloon.shape);
                it++;
            }
        }

        sf::Text loadingText(
            "Loading medieval survival\nwhen you updated this app,\nthis might take several minutes.\nDO NOT CLOSE",
            font);
        loadingText.setFillColor(sf::Color::White);
        loadingText.setCharacterSize((int)(minSize / 20.0f));

        // center text
        sf::FloatRect textRect = loadingText.getLocalBounds();
        loadingText.setOrigin(textRect.left + textRect.width / 2.0f,
                              textRect.top + textRect.height / 2.0f);
        loadingText.setPosition(window->getView().getCenter());

        window->draw(loadingText);

        // Display window contents on screen
        window->display();
    }
}

void loadResources()
{

    setCurrentThreadName(L"resource loader");

    createFoldersIfNotExists(savesFolder);
    //
    // fontFamily* family = new fontFamily(new texture(std::wstring(L"data\\ascii.png"), true));
    // defaultTheme = new theme(new font(family));
    currentApplication = new application(new gameForm(), gameName);
    loadingFinished = true;
}


void onError(const std::wstring& message)
{
	const std::string& messageToString = WStringToString(message);
	if constexpr (isDebugging)
	{
		throw std::runtime_error(messageToString);
	}
	else
	{
		if (currentWorld)
		{
			currentWorld->currentChat.addLine(message);
		}
		else
		{
			//TODO: message box cross-platform
			//int msgboxID = MessageBox(
			//	NULL,
			//	(LPCWSTR)message.c_str(),
			//	(LPCWSTR)gameName.c_str(),
			//	MB_ICONWARNING | MB_OK
			//);
			throw std::runtime_error(messageToString);
		}
	}
}

bool mainRunning = false;

int main(int argc, char *argv[])
{
    errorHandler.hook(onError);
    initialize();

    if (mainRunning)
    {
        //std::cout
        //    << "main function is being called again. this is probably because of a changed config in android";
        handleError(L"main is somehow called twice");
        // the main function is called
        return 0;
    }
    mainRunning = true;
    setCurrentThreadName(L"client thread");
    std::thread loadingThread(loadResources);
    showLoadingScreen();

    loadingThread.join();
    currentApplication->window = window;
    cint &result = currentApplication->run();
    //
    //
    delete currentApplication;
    //_CrtDumpMemoryLeaks();
    return result;
}