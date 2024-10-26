#pragma once
#include "playerSocket.h"
#include "constants.h"
constexpr fp defaultFPS = 60.0;
extern microseconds lastTickTimeMicroseconds;
extern seconds lastTickTime;
struct server {
	sf::TcpListener* listener = nullptr;
	sf::SocketSelector listenerSelector = sf::SocketSelector();
	std::vector<playerSocket*> clients = std::vector<playerSocket*>();
	bool everyoneSleeping = false;//to increase tick speed

	bool stopping = false;
    int lastRenderTick = 0;

	void execute();
	void renderClients();
	void stop();
	void tick();
	void updateToTime();
	void kick(playerSocket* socket);
	void addToServer(playerSocket* socket);
	microseconds msPerTick() const;
	human* findNearestPlayer(dimension* dimensionIn, cvec2& position);
	fp distanceToNearestPlayer(dimension* dimensionIn, cvec2& position);
	std::vector<human*> getPlayersInRadius(const dimension* dimensionIn, cvec2& position, cfp& radius);
	static void closeAllPorts();
};

extern server* currentServer;
void executeServer();
void createServerFromCurrentWorld();
playerSocket* listenForIncomingConnections();