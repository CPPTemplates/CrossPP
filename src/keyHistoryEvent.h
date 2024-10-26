#pragma once
struct keyHistoryEvent {
	vk key;
	bool down;//false = released
	keyHistoryEvent(cvk key = sf::Keyboard::Unknown, cbool& down = false) : key(key), down(down) {}
};