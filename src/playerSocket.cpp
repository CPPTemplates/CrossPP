#include "playerSocket.h"
#include "world.h"
#include <string>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include "constants.h"
#include "gameControl.h"
#include "array/arrayFunctions/arrayFunctions.h"
#include "globalFunctions.h"
#include "math/graphics/color/color.h"
#include "math/graphics/texture.h"
#include "math/rectangle/rectangletn.h"
#include "math/uuid.h"
#include "math/vector/vectn.h"
#include "human.h"
#include <thread>
#include <vector>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "application/thread/setThreadName.h"
#include "filesystem/serializer.h"
#include "nbt/serializeUUID.h"
#include "nbt/serializeColor.h"
#include "nbt/serializeList.h"
#include "math/sound/sound.h"
#include "serializeClientInput.h"

playerSocket::playerSocket(sf::TcpSocket *socket)
{
	screen = new gameControl(*this);
	this->s.socket = socket;
	write = false;

	ull num;
	// receive authentication packet.
	// wait for packet until timeout (fixes random connects pausing the server)
	// scope is for the selector
	{
		sf::SocketSelector selector;
		selector.add(*socket);
		// the packet should follow the connect shortly. we can wait safely, but one player can join at a time.
		if (!selector.wait(sf::milliseconds(5000)))
			return;
		s.receivePacket();
	}
	serialize(num);
	if (num != gameAuthNumber)
	{
		return;
	}
	authenticated = true;

	nbtCompound authPacket = nbtCompound(L"auth");
	streamSerializer streamS = streamSerializer(s, false, std::endian::big);
	authPacket.serialize(streamS);
	nbtSerializer inNBTSerializer = nbtSerializer(authPacket, false);

	uuid playerUUID;
	serializeNBTValue(inNBTSerializer, L"uuid", playerUUID);
	std::wstring playerName;
	inNBTSerializer.serializeValue(L"name", playerName);
	screen->player = player = new human(currentWorld->dimensions[(int)currentWorld->worldSpawnDimension], vec2(), *screen, playerName);

	std::wstring clientOSName;
	inNBTSerializer.serializeValue(L"OS", clientOSName);
	if (clientOSName == L"Android")
	{
		screen->addTouchInput();
		encoder.shouldCompress = socket->getRemoteAddress() != sf::IpAddress::LocalHost;
	}
	// enable to debug compression
	// encoder.shouldCompress = true;

	rectanglei2 screenRect = rectanglei2();
	serializeNBTValue(inNBTSerializer, L"screenSize", screenRect.size);
	screen->layout(screenRect);

	crectangle2 &relativeHitbox = player->calculateHitBox();

	player->position = currentWorld->worldSpawnPoint;
	player->newPosition = player->position;

	player->identifier = playerUUID;
	player->serialize(false);
}

void playerSocket::processSocketInput()
{
}

playerSocket::~playerSocket()
{
	s.socket->disconnect();
	delete s.socket;
}

void renderAsync(playerSocket *socket)
{
	setCurrentThreadName(L"renderer for " + socket->player->name);
	// we need to instantiate a context for openGL rendering. we don't need to do anything with it.
	// texture &currentContext = socket->contexts[socket->thread0DoubleBufferIndex];
	// currentContext.setActive(true);
	// sf::RenderTexture &newRenderResult = socket->doubleBuffer[socket->thread0DoubleBufferIndex];

	// create the rendertexture connected to the active context
	// newRenderResult.create(socket->screen->rect.size.x, socket->screen->rect.size.y, sf::ContextSettings());
	// newRenderResult.setActive(true);
	// TODO: check if it needs clear()
	texture *&currentRenderTarget = socket->buffer[0];

	if (!currentRenderTarget || currentRenderTarget->size != vect2<fsize_t>(socket->screen->rect.size))
	{
		if (currentRenderTarget)
		{
			delete currentRenderTarget;
		}
		currentRenderTarget = new texture(socket->screen->rect.size, false);
		// this way it will not be transparent
		currentRenderTarget->fill(colorPalette::black);
	}
	// we don't have to set the new render texture to active because its context is active already
	// newRenderResult.setActive(true);
	// newRenderResult.clear();
	socket->screen->render(cveci2(), *currentRenderTarget);
	// newRenderResult.setActive(false);
	// currentContext.setActive(false);
	// delete dummyTex;
	socket->screen->player->updateBodyParts(); // update body parts so the correct ear position can be obtained

	if (socket->sendRenderResultThread)
	{
		socket->sendRenderResultThread->join(); // wait until the last image has finished sending
		// socket->lastRenderResult->setActive(true);
		// delete socket->lastRenderResult;		// then delete the last render result
		delete socket->sendRenderResultThread;
	}
	// socket->lastRenderResult = newRenderResult;
	socket->write = true;

	// don't serialize this part async, as it doesn't take much time and might cause corruption when for example sounds are added mid-sending
	// send sounds
	// pointers because we pass this to another thread
	nbtCompound *compound = new nbtCompound(L"packetOut");
	nbtSerializer *outSerializer = new nbtSerializer(*compound, true);

	// always serialize. the target client may be on android or need an on-screen keyboard in another way
	bool wantsTextInput = socket->screen->wantsTextInput();
	outSerializer->serializeValue(L"wantsTextInput", wantsTextInput);

	outSerializer->serializeValue(L"paste", socket->screen->wantsClipboardInput);
	socket->screen->wantsClipboardInput = false;
	if (socket->screen->copyToClipboard.length())
	{
		outSerializer->serializeValue(L"copy", socket->screen->copyToClipboard);
		socket->screen->copyToClipboard.clear();
	}
	fp hearingRange2D = getHearingRange2D(socket->screen->visibleRange.x);
	serializeNBTValue(*outSerializer, L"hearingRange2D", hearingRange2D);

	vec3 earPosition = vec3(socket->screen->cameraPosition.x, socket->screen->cameraPosition.y, settings::soundSettings::headScreenDistance * socket->screen->visibleRange.x);
	// vec2 earPosition = socket->screen->player->getHeadPosition();
	serializeNBTValue(*outSerializer, L"earPosition", earPosition);
	// ear speed doesn't take into account that the ear can move closer and farther from the screen when zooming out
	vec3 earSpeed = vec3((socket->screen->player->newPosition - socket->screen->player->position) * ticksPerRealLifeSecond);
	serializeNBTValue(*outSerializer, L"earSpeed", earSpeed);

	if (outSerializer->push<nbtDataTag::tagList>(L"sounds"))
	{
		for (auto &data : socket->screen->dataToSend)
		{
			if (outSerializer->push())
			{
				data.serialize(*outSerializer);
				outSerializer->pop();
			}
		}
		outSerializer->pop();
	}

	socket->screen->serializeMusicPreference(*outSerializer);
	delete outSerializer;

	socket->screen->dataToSend.clear();

	// newRenderResult.setActive(false);
	socket->buffer.swap();
	// deletion of compound and outSerializer happen in the sendRenderResultAsync function
	socket->sendRenderResultThread = new std::thread(sendRenderResultAsync, socket, compound);

	sf::SocketSelector selector;
	selector.add(*socket->s.socket);
	selector.wait(sf::microseconds(1));
	fp receivedPackets = 0;
	// we don't wait for input, we just process the input in the next frame. too bad, it would cause server lag
	while (selector.isReady(*socket->s.socket))
	{ // the wait() of the socket is called in the main server loop once a tick
		// finally, send the packet with all the data of this render cycle to the player
		if (is_in(socket->s.receivePacket(), sf::Socket::Status::Disconnected, sf::Socket::Status::Error))
		{ // client disconnected?
			socket->shouldDisconnect = true;
			return;
		}
		else
		{
			// selector should continue looking if the socket is still ready
			socket->write = false;

			nbtCompound inCompound = nbtCompound(std::wstring(L"packetIn"));

			streamSerializer streamS = streamSerializer(socket->s, false, std::endian::big);
			inCompound.serialize(streamS);

			nbtSerializer currentNBTSerializer = nbtSerializer(inCompound, false);

			// read input while sending
			serializeClientInput(socket->screen->mostRecentInput,currentNBTSerializer);

			vect2<fsize_t> newScreenSize;
			serializeNBTValue(currentNBTSerializer, L"screenSize", newScreenSize);
			if (veci2(newScreenSize) != socket->screen->rect.size)
			{
				socket->screen->layout(crectanglei2(veci2(), veci2(newScreenSize)));
			}

			socket->screen->addClientInput(socket->screen->mostRecentInput);
			socket->screen->processInput();
			std::wstring clientClipboardText;
			if (currentNBTSerializer.serializeValue(L"clipboard", clientClipboardText))
			{
				socket->screen->paste(clientClipboardText);
			}
			selector.wait(sf::microseconds(1));
		}
		receivedPackets++;
	}
	socket->packetsReceivedPerSecond += receivedPackets;
	socket->packetsSentPerSecond++;
}

void sendRenderResultAsync(playerSocket *socket, nbtCompound *compound)
{
	socket->sending = true;
	setCurrentThreadName(L"screen compresser for " + socket->player->name);
	// sf::Context &currentContext = socket->contexts[thread1DoubleBufferIndex];
	// serialize screen and finally, send the packet

	// TODO: serialize the screen in 'sendrenderresultasync'. this is hard because s->write = false
	// we don't use the normal 'serialize' function so we don't have to use the 'write' boolean
	// std::stringstream compressedScreenPacket = std::stringstream();
	// sf::RenderTexture tex = sf::RenderTexture();
	// tex.create(socket->lastRenderResult->size.x, socket->lastRenderResult->size.y);
	// tex.clear();
	// sf::CircleShape shape(500);
	// shape.setPosition(sf::Vector2f(300, 300));
	// tex.draw(shape);

	// currentContext.setActive(true);
	// currentRenderResult.setActive(true); // this automatically activates the context of the render result
	// currentRenderResult->clear();
	// currentRenderResult.display();
	// sf::Image i = currentRenderResult.getTexture().copyToImage();
	// currentRenderResult.setActive(false);
	// currentContext.setActive(false);

	// uint pixelCount = i.getSize().x * i.getSize().y;

	// const textureRGB &texWithoutAlpha = textureRGB(currentRenderResult->size);
	// const colorRGB c = colorRGB();
	// colorRGB *ptr = texWithoutAlpha.baseArray;
	//// color *imagePtr = (color *)i.getPixelsPtr();
	// for (const color &c : *currentRenderResult)
	//{
	//	*ptr++ = colorRGB(c);
	// }

	// TODO: video streaming
	// we are guaranteed to be sending one packet at the same time. but we might be reading and sending at the same time
	if (socket->sendPacketThread)
	{
		socket->sendPacketThread->join();
		delete socket->sendPacketThread;
	}

	// socket->sendPacketThread = new std::thread(sendPacketAsync, socket);
	socket->sendPacketThread = new std::thread([compound, socket]()
											   { 
	setCurrentThreadName(L"packet sender for " + socket->player->name);
	// this way we can set the 'write' value to true
	//we can only start serializing once the old sending thread has finished
	streamSerializer streamS = streamSerializer(socket->s, true, std::endian::big);
	
	compound->serialize(streamS);
	texture *&currentRenderResult = socket->buffer[1];
	socket->encoder.addFrame(*currentRenderResult, streamS);
	
	delete compound;
	socket->s.sendPacket(); });
	socket->sending = false;
}

// void sendPacketAsync(playerSocket* socket) {
//
//	socket->s.sendPacket();
// }