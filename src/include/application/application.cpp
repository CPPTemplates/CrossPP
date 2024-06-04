#include "application.h"
#include "math/timemath.h"
#include "interaction.h"
#include "math/graphics/texture.h"
#include "optimization/stableTickLoop.h"
#include "control/control.h"
#include "math/graphics/texture.h"
#include <future>

#include <SFML/OpenGL.hpp>
#include "application/thread/setThreadName.h"
#include "array/arrayFunctions/sortedArray.h"
#include "control/eventTranslator.h"
// #include <gl/glew.h>

// works on windows only
// void application::changeKeyboardLayout()
//{
//	// Define the US English keyboard layout identifier
//	LPCWSTR US_ENGLISH_KEYBOARD_LAYOUT_ID = L"00000409";
//
//	// Load the US English keyboard layout
//	HKL usEnglishLayout = LoadKeyboardLayout(US_ENGLISH_KEYBOARD_LAYOUT_ID, KLF_ACTIVATE);
//
//	// Activate the US English keyboard layout
//	ActivateKeyboardLayout(usEnglishLayout, KLF_ACTIVATE);
//
//	// Example usage: Get the currently active keyboard layout
//	HKL activeLayout = GetKeyboardLayout(0);
//	std::wcout << L"Active Keyboard Layout: " << activeLayout << std::endl;
// }

constexpr int pixelMultiplier = onAndroid ? 4 : 1;
bool keyboardVisible = false;

int application::run()
{

    if (!window)
    {
        window = createWindow(name);
    }
    // initialize them AFTER the render window has been created, so they have a context
    windowTexture = new sf::Texture();
    windowSprite = new sf::Sprite();

    // settings = window->getSettings();
    // std::cout << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    // std::cout << settings.majorVersion << "." << settings.minorVersion << std::endl;

    // window->setFramerateLimit(60);
    //  ImGui::SFML::Init(window);

    // Create a texture with initial size

    // Create a sprite to display the texture

    // cap at 60fps
    cmicroseconds &frameTime = (microseconds)(1000000 / cappedFps);
    stableLoop loop = stableLoop(frameTime);
    // changeKeyboardLayout();
    // windowSprite->scale(1, -1);
    // windowSprite->move(0, (float)size.y);
    // windowSprite.scale(1, -1);
    // windowSprite.move(0, (float)size.y);
    layout(crectanglei2(cveci2(), veci2(window->getSize().x, window->getSize().y) / pixelMultiplier));
    mainForm->focus();
    renderThread = new std::thread(std::bind(&application::runGraphics, this));
    while (window->isOpen())
    {
        loop.waitTillNextLoopTime();
        startRendering->set_value(true);
        windowTexture->update((byte *)buffer[1]->baseArray);
        window->clear();
        // window->setActive(true);
        window->draw(*windowSprite);
        // ImGui::SFML::Render(window);
        window->display();

        // copy the texture to a temporary texture

        // auto asyncUpdate = [this, &tCopy]() {

        // window->setActive(false);
        //};
        // Start the asynchronous task
        // updateAsync = std::async(std::launch::async, asyncUpdate);
        // this will throw an error

        // Do stuff with graphics->colors
        finishedRendering->get_future().wait();
        finishedRendering = std::make_shared<std::promise<void>>();
        buffer.swap();

        cbool &wantsTextInput = mainForm->wantsTextInput();
        if (wantsTextInput != keyboardVisible)
        {
            sf::Keyboard::setVirtualKeyboardVisible(wantsTextInput);
            keyboardVisible = wantsTextInput;
        }

        processInput(); // process events from user right before the check if window->isOpen()
    }
    startRendering->set_value(false);
    finishedRendering->get_future().wait();
    delete windowTexture;
    delete windowSprite;
    delete window;
    return 0;
}

void application::runGraphics()
{
    setCurrentThreadName(L"calculation thread");
    while (startRendering->get_future().get())
    {
        // wait until signal to start processing is set

        startRendering = std::make_shared<std::promise<bool>>();

        texture &graphics = *buffer[0];
        mainForm->render(cveci2(0, 0), graphics);
        // Draw graphics->colors to window

        graphics.switchChannels(graphics.baseArray, 0, 2);
        finishedRendering->set_value();
    }
    finishedRendering->set_value();
}

void application::layout(crectanglei2 &newRect)
{
    sf::FloatRect visibleArea((float)newRect.x, (float)newRect.y, (float)newRect.w, (float)newRect.h);
    window->setView(sf::View(visibleArea));

    screenToApp = mat3x3::combine({mat3x3::scale(vec2(1.0 / pixelMultiplier)),
                                   mat3x3::translate(cvec2(0, -newRect.size.y)),
                                   mat3x3::mirror(axisID::y, 0)});

    window->setView(sf::View(sf::FloatRect((float)newRect.x, (float)newRect.y, (float)newRect.w,
                                           (float)newRect.h)));
    windowTexture->create(newRect.size.x, newRect.size.y);
    windowSprite->setTexture(*windowTexture, true);
    windowSprite->setScale(1, -1);
    windowSprite->setPosition(0, (float)newRect.size.y);

    buffer[0] = new texture(cvect2<fsize_t>(newRect.size));
    buffer[1] = new texture(cvect2<fsize_t>(newRect.size));
    mainForm->layout(crectanglei2(cveci2(), newRect.size));
}

void application::processInput()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            std::cout << "got close signal";
            if (mainForm->close())
            {
                window->close();
            }
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (addWithoutDupes(input.keysHolding, event.key.code))
            {
                listener.invoke(event);
            }
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            const auto &it = std::find(input.keysHolding.begin(), input.keysHolding.end(),
                                       event.key.code);
            if (it != input.keysHolding.end())
            {
                listener.invoke(event);
                input.keysHolding.erase(it);
            }
        }
        else if (is_in(event.type, sf::Event::TouchBegan, sf::Event::TouchMoved,
                       sf::Event::TouchEnded))
        {
            cveci2 &correctedPos = veci2(
                screenToApp.multPointMatrix(cvec2(event.touch.x, event.touch.y)));
            sf::Event correctedEvent = event;
            correctedEvent.touch.x = correctedPos.x;
            correctedEvent.touch.y = correctedPos.y;
            listener.invoke(correctedEvent);
        }
        else if (is_in(event.type, sf::Event::MouseButtonPressed,
                       sf::Event::MouseButtonReleased))
        {
            cveci2 &correctedPos = veci2(
                screenToApp.multPointMatrix(cvec2(event.mouseButton.x, event.mouseButton.y)));
            sf::Event correctedEvent = event;
            correctedEvent.mouseButton.x = correctedPos.x;
            correctedEvent.mouseButton.y = correctedPos.y;
            listener.invoke(correctedEvent);
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            cveci2 &correctedPos = veci2(
                screenToApp.multPointMatrix(cvec2(event.mouseMove.x, event.mouseMove.y)));
            sf::Event correctedEvent = event;
            correctedEvent.mouseMove.x = correctedPos.x;
            correctedEvent.mouseMove.y = correctedPos.y;
            listener.invoke(correctedEvent);
        }
        else if (event.type == sf::Event::Resized)
        {
            layout(rectanglei2(cveci2(), cveci2(event.size.width / pixelMultiplier,
                                                event.size.height / pixelMultiplier)));
        }
        else
        {
            // all other events are passed through directly
            listener.invoke(event);
        }
    }
}

application::application(form *mainForm, const std::wstring &name) : INamable(name)
{
    this->mainForm = mainForm;
    this->translator = new eventTranslator(*mainForm);
    listener.hook(&eventTranslator::processEvent, translator);
}

application::~application()
{
    delete mainForm;
}

sf::RenderWindow *application::createWindow(const std::wstring &name)
{
    sf::ContextSettings settings;

    settings.antialiasingLevel = 0;
    auto screenSize = sf::VideoMode::getDesktopMode();

    veci2 size = veci2(screenSize.width / pixelMultiplier, screenSize.height / pixelMultiplier);
    return new sf::RenderWindow(sf::VideoMode(size.x, size.y), WStringToString(name), onAndroid? sf::Style::Fullscreen :
                                (sf::Style::Close | sf::Style::Resize), settings);
}
