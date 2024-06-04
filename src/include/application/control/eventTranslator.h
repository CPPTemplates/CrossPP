#pragma once
#include "control.h"
#include <map>
struct eventTranslator
{
    control &receiver;
    eventTranslator(control &receiver) : receiver(receiver) {}
    void processEvent(const sf::Event& event);
    std::map<mb, veci2> dragStartPositions;
};