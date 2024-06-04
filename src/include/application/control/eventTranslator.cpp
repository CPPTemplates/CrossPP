#include "eventTranslator.h"

void eventTranslator::processEvent(const sf::Event &event)
{
    auto processMouseDown = [this](cveci2 &mousePos, cmb &button)
    {
        receiver.mouseDown(
            mousePos,
            button);
        dragStartPositions[button] = mousePos;
    };
    auto processMouseMove = [this](cveci2 &mousePos, cmb &button)
    {
        receiver.mouseMove(
            mousePos,
            button);
        receiver.drag(dragStartPositions[button], mousePos, button);
    };
    auto processMouseUp = [this](cveci2 &mousePos, cmb &button)
    {
        receiver.mouseUp(
            mousePos,
            button);
        receiver.drop(dragStartPositions[button], mousePos, button);
        dragStartPositions.erase(button);
    };
    if (event.type == sf::Event::KeyPressed)
    {
        receiver.keyDown(event.key.code);
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        receiver.keyUp(event.key.code);
    }
    else if (event.type == sf::Event::TextEntered)
    {
        receiver.enterText(event.text.unicode);
    }
    else if (event.type == sf::Event::TouchBegan)
    {
        processMouseDown(veci2(event.touch.x, event.touch.y), (mb)event.touch.finger);
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
        processMouseDown(veci2(event.mouseButton.x, event.mouseButton.y), (mb)event.mouseButton.button);
    }
    else if (event.type == sf::Event::TouchMoved)
    {
        processMouseMove(
            cveci2(event.touch.x, event.touch.y), (mb)event.touch.finger);
    }
    else if (event.type == sf::Event::TouchEnded)
    {
        processMouseUp(
            cveci2(event.touch.x, event.touch.y),
            (mb)event.touch.finger);
    }

    else if (event.type == sf::Event::MouseButtonReleased)
    {
        processMouseUp(
            cveci2(event.mouseButton.x, event.mouseButton.y),
            (mb)event.mouseButton.button);
    }
    else if (event.type == sf::Event::MouseWheelScrolled)
    {
        receiver.scroll(cveci2(event.mouseWheelScroll.x, event.mouseWheelScroll.y),
                        (int)event.mouseWheelScroll.delta);
    }
    else if (event.type == sf::Event::Resized)
    {
        receiver.layout(rectanglei2(cveci2(), cveci2(event.size.width,
                                                     event.size.height)));
    }
    else if (event.type == sf::Event::LostFocus)
    {
        receiver.lostFocus();
    }
    else if (event.type == sf::Event::GainedFocus)
    {
        receiver.focus();
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        processMouseMove(cveci2(event.mouseMove.x, event.mouseMove.y), noButton);
    }
}