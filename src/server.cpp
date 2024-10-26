#include "server.h"
#include "human.h"
#include "world.h"
#include "dimension.h"
#include "end.h"
#include <thread>
#include "client.h"
#include "benchMark.h"
#include <optimization/stableTickLoop.h>
#include "cpuUsageID.h"
#include <future>
#include "application/thread/setThreadName.h"
server *currentServer = nullptr;
std::thread *serverThread = nullptr;

auto listeners = std::unordered_map<port, sf::TcpListener *>();

void server::execute()
{
	setCurrentThreadName(L"server main thread");
	sf::Socket::Status portStatus = sf::Socket::Status::Done;
	// bind the listener to a port
	if (!(listener = listeners[defaultPort]))
	{
		listeners[defaultPort] = new sf::TcpListener();
		listener = listeners[defaultPort];
		// it doesn't work like this:
		// listener = listeners[defaultPort] = new sf::TcpListener();
		while (!stopping && (portStatus = listener->listen(defaultPort)))
		{
			// cannot listen at the moment
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
	if (portStatus == sf::Socket::Done)
	{
		listenerSelector.add(*listener);

		stableLoop loop = stableLoop(1000000 / (microseconds)defaultFPS);
		// connectionManagerThread = new std::thread(listenForIncomingConnections);

		std::future<playerSocket *> newPlayerSocket = std::async(&listenForIncomingConnections);

		// at least one tick
		lastTickTimeMicroseconds = getmicroseconds() - microSecondsPerTick;
		while (!stopping)
		{ // server loop
			loop.waitTillNextLoopTime();

			updateToTime();

			// render
			renderClients();
			currentBenchmark->addBenchmarkPoint(cpuUsageID::networking);

			for (size_t i = 0; i < clients.size();)
			{
				if (clients[i]->shouldDisconnect)
				{
					kick(clients[i]);
				}
				else
				{
					i++;
				}
			}

			// accept connection
			if (newPlayerSocket.valid())
			{
				if (playerSocket *newSocket = newPlayerSocket.get())
				{
					// check if not double joining
					for (auto const &cl : clients)
					{
						if (cl->player->identifier == newSocket->player->identifier)
						{
							currentWorld->currentChat.addLine(newSocket->player->name +
															  L" tried to join, but a player with UUID " +
															  (std::wstring)cl->player->identifier +
															  L" is online already");
							delete newSocket;
							newSocket = nullptr;
							break;
						}
					}
					if (newSocket)
					{
						addToServer(newSocket);
					}
				}
				newPlayerSocket = std::async(&listenForIncomingConnections);
			}
			// connectionManagerThread->join();
			// delete connectionManagerThread;
			// connectionManagerThread = new std::thread(listenForIncomingConnections);

			currentBenchmark->addBenchmarkPoint(cpuUsageID::miscellaneous);
		}
		// delete connectionManagerThread;
		const std::vector<playerSocket *> copy = clients;
		for (playerSocket *const &c : copy)
		{
			kick(c);
		}
		// for the elegance
		// first remove the listener from the selector!
		listenerSelector.remove(*listener);
		// stopping
		currentWorld->serialize(true);
	}
	delete currentWorld;
	currentWorld = nullptr;
	delete this;
	currentServer = nullptr;
}

void server::renderClients()
{
	currentBenchmark->addBenchmarkPoint(cpuUsageID::drawing);

	std::vector<std::thread *> threads = std::vector<std::thread *>();
	for (auto c : clients)
	{
		constexpr fp multiplier = 1 - (1 / defaultFPS);
		c->packetsReceivedPerSecond *= multiplier;
		c->packetsSentPerSecond *= multiplier;
		// sync frame rate with ticks
		// when the client is not keeping up with the server, send less packets, less to render. yay!
		// synchronize the client's screen with the ticks
		//cbool &clientOverloaded = c->packetsSentPerSecond - c->packetsReceivedPerSecond > 10; // || c->packetsSentPerSecond < 25;
		if (c->sending)
		{
			// not done with last render yet. we might have to wait (and lag the server) if we render this one. better not render
		}
		else
		{
			if (!c->encoder.shouldCompress || currentWorld->ticksSinceStart > lastRenderTick)
			{
				// mobile device, don't send that many frames to avoid overloading it
				threads.push_back(new std::thread(renderAsync, c));
			}
		}
	}
	// wait until all screens have rendered
	for (auto t : threads)
	{
		t->join();
		delete t;
	}
	lastRenderTick = currentWorld->ticksSinceStart;
	currentBenchmark->removeOldBenchmarks(); // the rendering time will be displayed in the next render session
	currentBenchmark->addBenchmarkPoint(cpuUsageID::miscellaneous);
}

void server::stop()
{
	stopping = true;
	// wait until the server has stopped
	serverThread->join();
}

void server::tick()
{
	currentBenchmark->addBenchmarkPoint(cpuUsageID::chunkLoading);

	constexpr fp playerLoadDistance = 0x20; // the distance which should always be loaded in around the player, with or without a camera looking at it

	// visiblerange * 2 because the camera can load in 2x further
	// keep chunks around the player loaded. the visible range will only update every frame, but that doesn't matter much
	bool inEnd = false;
	everyoneSleeping = true;
	for (const auto *client : clients)
	{
		if (!inEnd && client->player->dimensionIn->identifier == dimensionID::end)
		{
			inEnd = true;
		}
		if (!client->player->sleeping)
		{
			everyoneSleeping = false;
		}
		client->player->dimensionIn->keepPlayerLoaded(dimension::getTouchingChunkCoordinateRange(crectangle2(client->screen->cameraPosition - client->screen->visibleRange, client->screen->visibleRange * 2)));
		client->player->dimensionIn->keepPlayerLoaded(dimension::getTouchingChunkCoordinateRadius(client->player->position, playerLoadDistance));
	}
	if (inEnd)
	{
		end *currentEnd = (end *)currentWorld->dimensions[(int)dimensionID::end];
		if (currentEnd->dragonAlive)
		{
			currentEnd->keepPlayerLoaded(dimension::getTouchingChunkCoordinateRange(crectangle2(-mainEndIslandMaxRadius, 0, mainEndIslandMaxRadius * 2, 0x100)));
		}
	}
	currentBenchmark->addBenchmarkPoint(cpuUsageID::miscellaneous);
	currentWorld->tick();

	// also set them to false if the world does not have focus, so it will not be placing random blocks
	for (auto c : clients)
	{
		c->screen->clearTemporaryData();
		c->screen->touchStarted = false;
		c->screen->touchEnded = false;
		fillAllElements(c->screen->clickedFocused, false);
		// fillAllElements(c->screen->holding, false);
	}

	// rightClicked = false;
	// leftClicked = false;
	// middleClicked = false;
	// rightClickReleased = false;
	// leftClickReleased = false;
	// middleClickReleased = false;
	// holdingLeftClick = false;
	// holdingRightClick = false;
	// holdingMiddleClick = false;
}

void server::updateToTime()
{
	microseconds s = getmicroseconds();

	// first update the transform, so all entities know the drawing positions already
	int tickCount = 0;
	cint maxTicksPerFrame = 0x4; // to avoid an endless lag loop
	while (s > lastTickTimeMicroseconds + msPerTick())
	{
		if (tickCount >= maxTicksPerFrame)
		{
			lastTickTimeMicroseconds = s;
			lastTickTime = microsectosec(lastTickTimeMicroseconds);
			break;
		}
		tick();
		lastTickTimeMicroseconds += msPerTick();
		lastTickTime = microsectosec(lastTickTimeMicroseconds);
		tickCount++;
	}
	// auto save here
	// save the world, so if the world crashes before the first save, we still have it
	if ((currentWorld->ticksSinceStart - currentWorld->lastAutoSaveTick) > (5 * ticksPerRealLifeMinute) || currentWorld->lastAutoSaveTick == 0) // save every 5 minutes and upon world creation
	{
		currentWorld->lastAutoSaveTick = currentWorld->ticksSinceStart;
		currentWorld->serialize(true);
		for(playerSocket* p : clients){
			//save players
			p->player->serialize(true);
		}
	}
}

void server::kick(playerSocket *socket)
{
	socket->player->serialize(true);
	socket->player->despawn = true;
	// selector.remove(*(socket->s.socket));
	clients.erase(find(clients, socket));
	socket->s.socket->disconnect();
}

void server::addToServer(playerSocket *socket)
{
	socket->player->addToWorld();
	clients.push_back(socket);
}

microseconds server::msPerTick() const
{
	return everyoneSleeping ? microSecondsPerTick / 0x10 : microSecondsPerTick;
}

human *server::findNearestPlayer(dimension *dimensionIn, cvec2 &position)
{
	human *nearestPlayer = nullptr;
	fp nearestDistanceSquared = INFINITY;
	for (auto p : clients)
	{
		if (p->player->dimensionIn == dimensionIn)
		{
			fp dist = (p->player->position - position).lengthSquared();
			if (dist < nearestDistanceSquared)
			{
				nearestDistanceSquared = dist;
				nearestPlayer = p->player;
			}
		}
	}

	return nearestPlayer;
}

fp server::distanceToNearestPlayer(dimension *dimensionIn, cvec2 &position)
{
	const human *nearestPlayer = findNearestPlayer(dimensionIn, position);
	return nearestPlayer ? (nearestPlayer->position - position).length() : INFINITY;
}

std::vector<human *> server::getPlayersInRadius(const dimension *dimensionIn, cvec2 &position, cfp &radius)
{
	cfp &radiusSquared = radius * radius;
	std::vector<human *> result = std::vector<human *>();
	for (auto p : clients)
	{
		human *const &h = p->player;
		if (h->dimensionIn == dimensionIn)
		{
			if ((h->position - position).lengthSquared() < radiusSquared)
			{
				result.push_back(h);
			}
		}
	}
	return result;
}

void server::closeAllPorts()
{
	for (auto l : listeners)
	{
		l.second->close();
		delete l.second;
	}
}

void executeServer()
{
	currentServer->execute();
}

void createServerFromCurrentWorld()
{
	currentServer = new server();
	serverThread = new std::thread(executeServer); // starts the server
	currentClient->connectToServer(serverData());
}

playerSocket *listenForIncomingConnections()
{
	// seconds tickTimeLeft = lastTickTime + microsectosec(msPerTick()) - getSeconds();
	// dummy time

	if (currentServer->listenerSelector.wait(sf::microseconds(1)))
	{

		// received something
		// if (selector.isReady(listener))
		//{
		// accept a new connection
		sf::TcpSocket *clientSocket = new sf::TcpSocket();
		if (currentServer->listener->accept(*clientSocket) != sf::Socket::Done)
		{
			// error...
		}

		playerSocket *socket = new playerSocket(clientSocket);

		if (socket->authenticated)
		{
			return socket;
			// selector.add(*clientSocket);
		}
		else
		{
			// this client didn't authenticate
			delete socket;
		}
		//}
	}
	return nullptr;
	// for (auto c : clients) {
	//	if (selector.isReady(*c->s.socket)) {
	//		//the client has sent us data
	//		c->processSocketInput();
	//	}
	// }
}
