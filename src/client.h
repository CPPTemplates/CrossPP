#pragma once
#include "application/control/control.h"
#include "serverData.h"
#include "socketContainer.h"
#include "network/client/clientInput.h"
#include "playerData.h"
#include "math/graphics/video/videoEncoder.h"
#include <queue>
#include <memory>
#include <future>
struct client : control, socketContainer {
	serverData currentServerData = serverData();
	vec3 earPosition = vec3();
	vec3 earSpeed = vec3();
	fp hearingRange2D = 0;
    bool socketWantsTextInput = false;
	bool socketWantsClipboardInput = false;

	clientInput currentInput;
	playerData data;
	sf::SocketSelector selector = sf::SocketSelector();
	videoEncoder decoder = videoEncoder();
    std::queue<std::shared_ptr<sf::Packet>> receivedPackets = std::queue<std::shared_ptr<sf::Packet>>();
    std::mutex receivedPacketsMutex;
    std::shared_ptr<std::promise<void>> packetWaiter = std::make_shared<std::promise<void>>();
    sf::Socket::Status status = sf::Socket::Done;
    //std::promise<std::shared_ptr<sf::Packet>> receivePacket = std::promise<std::shared_ptr<sf::Packet>>();
    bool lagging = false;

	virtual void render(cveci2& position, const texture& renderTarget) override;
	client();
	virtual void mouseDown(cveci2& position, cmb& button) override;
	virtual void mouseUp(cveci2& position, cmb& button) override;
	virtual void scroll(cveci2& position, cint& scrollDelta) override;
	virtual void enterText(cuint& keyCode) override;

	virtual void mouseMove(cveci2& position, cmb& button) override;
	virtual void keyDown(cvk& keyCode) override;
	virtual void keyUp(cvk& keyCode) override;
	virtual void lostFocus() override;
    bool wantsTextInput() const override;

    void retrievePacketsAsync();

    bool connectToServer(const serverData& server);
	void sendPacket(const texture &renderTarget);
	void processIncomingPackets(const texture &renderTarget);
    void addEvent(const sf::Event& e);
};
extern client* currentClient;
