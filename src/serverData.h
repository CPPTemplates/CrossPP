#pragma once
#include <SFML/Network/IpAddress.hpp>
#include "constants.h"
#include "nbt/nbtSerializable.h"
struct serverData : nbtSerializable {
	std::wstring serverName = L"local";
	port serverPort = defaultPort;
	sf::IpAddress serverIPAddress = sf::IpAddress::LocalHost;
	virtual void serializeValue(nbtSerializer& s) override;

	inline bool operator==(const serverData& other) const {
		return serverName == other.serverName && serverPort == other.serverPort && serverIPAddress == other.serverIPAddress;
	}
};