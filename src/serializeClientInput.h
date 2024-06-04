#pragma once
#include "network/client/clientInput.h"
#include "nbt/nbtSerializer.h"
inline bool serializeClientInput(clientInput& input, nbtSerializer &s)
{
    if(s.push(L"events")){
        size_t size = input.eventHistory.size();
        s.serializeValue(L"size", size);
        if (!s.write)
        {
            input.eventHistory.resize(size);
        }
        for (size_t i = 0; i < size; i++)
        {
            if (s.push(std::to_wstring(i)))
            {
                //typedef std::underlying_type_t<sf::Event::EventType> ut;
                //ut &k = (ut &)eventHistory[i].type;
                s.serializeValue(L"type", input.eventHistory[i].type);
                switch (input.eventHistory[i].type) {
                    case sf::Event::TouchMoved:
                    case sf::Event::TouchBegan:
                    case sf::Event::TouchEnded:
                        s.serializeValue(L"x", input.eventHistory[i].touch.x);
                        s.serializeValue(L"y", input.eventHistory[i].touch.y);
                        s.serializeValue(L"finger", input.eventHistory[i].touch.finger);

                        break;
                    case sf::Event::MouseButtonPressed:
                    case sf::Event::MouseButtonReleased:
                        s.serializeValue(L"x", input.eventHistory[i].mouseButton.x);
                        s.serializeValue(L"y", input.eventHistory[i].mouseButton.y);
                        s.serializeValue(L"button", input.eventHistory[i].mouseButton.button);
                        break;
                    case sf::Event::MouseMoved:
                        s.serializeValue(L"x", input.eventHistory[i].mouseMove.x);
                        s.serializeValue(L"y", input.eventHistory[i].mouseMove.y);
                        break;
                    case sf::Event::KeyPressed:
                    case sf::Event::KeyReleased:
                        //we don't need to serialize the scan code, that is the local key code.
                        //when reading this data on another device, the result would be completely different.
                        s.serializeValue(L"alt", input.eventHistory[i].key.alt);
                        s.serializeValue(L"control", input.eventHistory[i].key.control);
                        s.serializeValue(L"shift", input.eventHistory[i].key.shift);
                        s.serializeValue(L"code", input.eventHistory[i].key.code);
                        break;
                    case sf::Event::SensorChanged:
                        s.serializeValue(L"x", input.eventHistory[i].sensor.x);
                        s.serializeValue(L"y", input.eventHistory[i].sensor.y);
                        s.serializeValue(L"z", input.eventHistory[i].sensor.z);
                        break;
                    case sf::Event::MouseWheelScrolled:
                        s.serializeValue(L"x", input.eventHistory[i].mouseWheelScroll.x);
                        s.serializeValue(L"y", input.eventHistory[i].mouseWheelScroll.y);
                        s.serializeValue(L"delta", input.eventHistory[i].mouseWheelScroll.delta);
                        s.serializeValue(L"wheel", input.eventHistory[i].mouseWheelScroll.wheel);
                        break;
                    case sf::Event::TextEntered:
                        s.serializeValue(L"unicode", input.eventHistory[i].text.unicode);
                        break;
                    case sf::Event::Resized:
                        s.serializeValue(L"width", input.eventHistory[i].size.width);
                        s.serializeValue(L"height", input.eventHistory[i].size.height);
                        break;
                    case sf::Event::MouseEntered://no data associated with these
                    case sf::Event::MouseLeft:
                    case sf::Event::Closed:
                    case sf::Event::LostFocus:
                    case sf::Event::GainedFocus:
                    default:
                    break;
                }
                s.pop();
            }
        }
        s.pop();
    }
	serializeNBTValue(s, L"mouse position", input.mousePositionPixels);
	if (s.push(L"mouse buttons"))
	{
		s.serializeArray(L"clicked", (sbyte*)input.clicked, (size_t)sf::Mouse::ButtonCount);
		s.serializeArray(L"released", (sbyte*)input.clickReleased, (size_t)sf::Mouse::ButtonCount);
		s.serializeArray(L"holding", (sbyte*)input.holding, (size_t)sf::Mouse::ButtonCount);
		s.serializeValue(L"scrolldelta", input.scrollDelta);
		s.pop();
	}
	// s.serialize(keyDownHistory);
	if (s.push(L"keys"))
	{
		serializeNBTValue(s, L"down", input.keysDown);
		serializeNBTValue(s, L"up", input.keysUp);
		serializeNBTValue(s, L"holding", input.keysHolding);
		s.pop();
	}
	s.serializeValue(L"text entered", input.textEntered);

	return true;
}