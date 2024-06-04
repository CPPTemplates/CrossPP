#pragma once
#include <SFML/Window.hpp>
#include "constants.h"

static constexpr int hotbarSlotKeys[StandardInventoryColumnCount]
{
	sf::Keyboard::Num1,
	sf::Keyboard::Num2,
	sf::Keyboard::Num3,
	sf::Keyboard::Num4,
	sf::Keyboard::Num5,
	sf::Keyboard::Num6,
	sf::Keyboard::Num7,
	sf::Keyboard::C,
	sf::Keyboard::F,
};

enum class keyID
{
	hotbarSlot0 = hotbarSlotKeys[0],
	hotbarSlot1 = hotbarSlotKeys[1],
	hotbarSlot2 = hotbarSlotKeys[2],
	hotbarSlot3 = hotbarSlotKeys[3],
	hotbarSlot4 = hotbarSlotKeys[4],
	hotbarSlot5 = hotbarSlotKeys[5],
	hotbarSlot6 = hotbarSlotKeys[6],
	hotbarSlot7 = hotbarSlotKeys[7],
	hotbarSlot8 = hotbarSlotKeys[8],
	left = sf::Keyboard::A,
	right = sf::Keyboard::D,
	up = sf::Keyboard::W,
	down = sf::Keyboard::S,
	inventory = sf::Keyboard::E,
	drop = sf::Keyboard::Q,
	camera = sf::Keyboard::F,
	hit = sf::Mouse::Left,
	use = sf::Mouse::Right,
	jump = sf::Keyboard::Space,
	sprint = sf::Keyboard::LControl,
	sneak = sf::Keyboard::LShift,
	exit = sneak,
	commandLine = sf::Keyboard::Slash,//backslash or question mark
	text = sf::Keyboard::T,
	headUpDisplay = sf::Keyboard::F1,
	screenshot = sf::Keyboard::F2,
	debug = sf::Keyboard::F3,
	escape = sf::Keyboard::Escape,
	renderHitboxes = sf::Keyboard::B
};