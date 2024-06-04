#pragma once
#include <SFML/Window.hpp>
#include "math/vector/vectn.h"

struct nbtSerializer;
struct socketContainer;
struct clientInput {
	veci2 mousePositionPixels = veci2();

	int scrollDelta = 0;

	bool clicked[sf::Mouse::ButtonCount]{};
	bool clickReleased[sf::Mouse::ButtonCount]{};
	bool holding[sf::Mouse::ButtonCount]{};

	//the keys which were pressed (0 = start)
	std::wstring textEntered = std::wstring();

	std::vector<vk> keysDown = std::vector<vk>();
	std::vector<vk> keysUp = std::vector<vk>();
	std::vector<vk> keysHolding = std::vector<vk>();

    std::vector<sf::Event> eventHistory = std::vector<sf::Event>();

	void addClientInput(const clientInput& newInput);
	bool holdingDownKey(cvk& key) const;
	bool pressedKey(cvk& key) const;
	//clear all data which should not be there if this input gets processed twice
	void clearTemporaryData();
};