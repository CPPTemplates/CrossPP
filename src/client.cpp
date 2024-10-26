#include "client.h"
#include "array/arrayFunctions/sortedArray.h"
#include "soundPacket.h"
#include "soundCollection.h"
#include <memory>
#include "mainMenu.h"
#include "server.h"
#include "worldSelector.h"
#include "keyHistoryEvent.h"
#include "filesystem/fpng/fpng.h"
#include <cstdint>
#include <vector>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Time.hpp>
#include "constants.h"
#include "application/mouseButton.h"
#include "globalFunctions.h"
#include "math/graphics/color/color.h"
#include "math/graphics/texture.h"
#include "math/random/random.h"
#include "math/vector/vectn.h"
#include "serverData.h"
#include "sound/soundHandler2D.h"
#include "folderList.h"
#include "math/timemath.h"
#include "main.h"
#include <filesystem/filemanager.h>
#include "nbt/serializeColor.h"
#include "nbt/serializeUUID.h"
#include "include/filesystem/fileFunctions.h"
#include "nbt/serializeList.h"
#include "include/application/thread/setThreadName.h"
#include "musicManager.h"
#include "keyID.h"
#include "serializeClientInput.h"
#include "nbt/serializeList.h"

miliseconds lastScreenshotTime = 0;

void client::render(cveci2 &position, const texture &renderTarget)
{
    write = true;

    cbool &takeScreenShot = currentInput.pressedKey((vk)keyID::screenshot);

    sendPacket(renderTarget);

    processIncomingPackets(renderTarget);
    if (takeScreenShot)
    {
        // take screenshot

        createFoldersIfNotExists(screenshotsFolder);
        renderTarget.Flip();
        for (color &c : renderTarget)
        {
            c.a() = color::maxValue;
        }

        renderTarget.Save(screenshotsFolder / (timeToString(L"%Y-%m-%d_%H.%M.%S") + L".png"));
        lastScreenshotTime = getMiliseconds();
    }
    // 0.5 seconds flash
    if (getMiliseconds() - lastScreenshotTime < 500)
    {
        // renderTarget.Save(screenshotsFolder / timeToString(L"%Y-%m-%d_%H.%M.%S") + L".bmp");//for testing
        renderTarget.fill(colorPalette::white); // flash effect
    }
}

client::client()
{
    if (!data.serialize(false))
    {
        data.id = randomUUID(currentRandom);

        // while (true) {
        //	output(randomName(currentRandom) + L"\n");
        // }

        data.name = randomName(currentRandom);
        data.serialize(true);
    }
}

void client::mouseDown(cveci2 &position, cmb &button)
{
    currentInput.clicked[button] = true;
    currentInput.holding[button] = true;
    currentInput.mousePositionPixels = position;
}

void client::mouseUp(cveci2 &position, cmb &button)
{
    currentInput.clickReleased[button] = true;
    currentInput.holding[button] = false;
    currentInput.mousePositionPixels = position;
}

void client::scroll(cveci2 &position, cint &scrollDelta)
{
    currentInput.scrollDelta += scrollDelta;
}

void client::enterText(cuint &keyCode)
{
    currentInput.textEntered += (wchar_t)keyCode;
}

void client::mouseMove(cveci2 &position, cmb &button)
{
    currentInput.mousePositionPixels = position;
}

void client::keyDown(cvk &keyCode)
{
    addWithoutDupes(currentInput.keysDown, keyCode);
    currentInput.keysHolding.push_back(keyCode);
}

void client::keyUp(cvk &keyCode)
{
    currentInput.keysUp.push_back(keyCode);
    const auto &it = std::find(currentInput.keysHolding.begin(), currentInput.keysHolding.end(),
                               keyCode);
    if (it != currentInput.keysHolding.end())
    {
        currentInput.keysHolding.erase(it);
    }
}

void client::lostFocus()
{
    currentInput.keysHolding.clear();
}

bool client::wantsTextInput() const
{
    return socketWantsTextInput;
}

bool client::connectToServer(const serverData &server)
{
    currentApplication->listener.hook(&client::addEvent, this);

    s.socket = new sf::TcpSocket();
    int tries = 0;
    do
    {
        if (tries)
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        status = s.socket->connect(server.serverIPAddress, server.serverPort,
                                   sf::seconds(5));

    } while (++tries < 2 && status != sf::Socket::Done && server.serverIPAddress == sf::IpAddress::LocalHost);

    if (status != sf::Socket::Done)
    {
        if (status == sf::Socket::Error)
        {
        }
        else if (status == sf::Socket::NotReady)
        {
            // not ready means that there's probably nothing on this port. return
        }
        return false;
        // error...
    }

    write = true;
    // ull num = gameAuthNumber;
    // unsigned long long num = gameAuthNumber;
    // construct authentication packet
    ull num = gameAuthNumber;
    serialize(
        num); // this is to distinguish between random things pinging this port and clients trying to connect
    nbtCompound authPacket = nbtCompound(L"auth");
    nbtSerializer outSerializer = nbtSerializer(authPacket, true);
    serializeNBTValue(outSerializer, L"uuid", data.id);
    outSerializer.serializeValue(L"name", data.name);
    std::wstring osName = onAndroid ? L"Android" : onWindows ? L"Windows"
                                               : onMac       ? L"Mac"
                                               : onLinux     ? L"Linux"
                                                             : L"Other";
    outSerializer.serializeValue(L"OS", osName);
    serializeNBTValue(outSerializer, L"screenSize", this->rect.size);
    streamSerializer streamS = streamSerializer(s, true, std::endian::big);
    authPacket.serialize(streamS);
    s.sendPacket();
    // reset
    // selector.clear();
    selector.add(*s.socket);
    std::thread receiveThread = std::thread(std::bind(&client::retrievePacketsAsync, this));
    receiveThread.detach();
    return true;
}

void client::sendPacket(const texture &renderTarget)
{
    nbtCompound outCompound = nbtCompound(std::wstring(L"packetOut"));
    nbtSerializer outSerializer = nbtSerializer(outCompound, true);

    serializeClientInput(currentInput, outSerializer);
    auto screenSize = renderTarget.size;
    serializeNBTValue(outSerializer, L"screenSize", screenSize);

    if (socketWantsClipboardInput)
    {
        std::wstring clipboardString = sf::Clipboard::getString().toWideString();
        outSerializer.serializeValue(L"clipboard", clipboardString);
    }

    streamSerializer streamS = streamSerializer(s, true, std::endian::big);
    outCompound.serialize(streamS);
    // this is thread safe, as it will swap the packet right at the start of the sendpacket() function
    s.sendPacket();
    // std::thread([this]{
    // }).detach();
    currentInput.clearTemporaryData();
}

void client::processIncomingPackets(const texture &renderTarget)
{
    write = false;

    nbtSerializer *inSerializer = nullptr;
    nbtCompound *inCompound = nullptr;

    receivedPacketsMutex.lock();
    size_t receivedPacketsCount = receivedPackets.size();
    receivedPacketsMutex.unlock();

    if (!receivedPacketsCount && !status)
    {
        // copy the shared_ptr so it will stay alive, even when it is replaced in the list
        auto copy = packetWaiter;
        // wait till a packet is received
        copy->get_future().get();
        receivedPacketsMutex.lock();
        receivedPacketsCount = receivedPackets.size();
        receivedPacketsMutex.unlock();
    }

    // receive packets, but don't process the screen if another packet is waiting, so we can catch up
    // dividing by 4 to make the experience more smooth
    // math::ceil<size_t,fp>(size / 4.0)
    // size
    for (size_t amountToPopLeft = math::minimum(receivedPacketsCount,
                                                (size_t)2);
         amountToPopLeft; amountToPopLeft--)
    {
        receivedPacketsMutex.lock();
        auto packet = receivedPackets.front();
        receivedPackets.pop();
        receivedPacketsMutex.unlock();
        delete inCompound;
        inCompound = new nbtCompound(std::wstring(L"packetIn"));
        s.setReceivingPacket(&*packet);

        streamSerializer streamS = streamSerializer(s, false, std::endian::big);
        inCompound->serialize(streamS);
        s.receivingPacket = nullptr;

        delete inSerializer;
        inSerializer = new nbtSerializer(*inCompound, false);

        if constexpr (onAndroid)
        {
            inSerializer->serializeValue(L"wantsTextInput", socketWantsTextInput);
        }
        inSerializer->serializeValue(L"paste", socketWantsClipboardInput);
        std::wstring copiedText;
        if (inSerializer->serializeValue(L"copy", copiedText))
        {
            sf::Clipboard::setString(WStringToString(copiedText));
        }
        // serialize colors of the screen
        // sounds
        serializeNBTValue(*inSerializer, L"hearingRange2D", hearingRange2D);
        serializeNBTValue(*inSerializer, L"earPosition", earPosition);
        serializeNBTValue(*inSerializer, L"earSpeed", earSpeed);
        
        if (inSerializer->push<nbtDataTag::tagList>(L"sounds"))
        {
            const std::vector<nbtData *> &serializedSoundList = inSerializer->getChildren();
            for (const nbtData *serializedSoundData : serializedSoundList)
            {
                if (inSerializer->push(serializedSoundData))
                {

                    soundPacket sp = soundPacket();

                    if (!sp.serialize(*inSerializer))
                    {
                        inSerializer->pop();
                        break;
                    }

                    if (globalSoundCollectionList.contains(sp.key))
                    {
                        // we assume that the sound is a soundCollection, not a musicCollection
                        const soundCollection *collection = (soundCollection *)globalSoundCollectionList[sp.key];
                        // make sure that the sound index is in the right range.
                        // we might have added or removed sounds in the mean time
                        // by using %, we make every sound that sounds the same for other clients also sound the same for this client
                        sp.soundIndex = sp.soundIndex % (int)collection->audioToChooseFrom.size();

                        const auto buffer = collection->audioToChooseFrom[sp.soundIndex];
                        if (!buffer)
                        {
                            handleError(L"audio not loaded properly");
                        }
                        std::shared_ptr<sound2d> soundToPlay = std::make_shared<sound2d>(
                            buffer,
                            sp.position, sp.volume, sp.pitch, true);
                        handler.playAudio(soundToPlay);
                    }

                    inSerializer->pop();
                }
            }
            inSerializer->pop();
        }
        if (inSerializer->push(L"music"))
        {
            std::wstring newMusicKey;
            if (inSerializer->serializeValue(L"replace", newMusicKey))
            {
                const musicCollection *collection = (musicCollection *)globalSoundCollectionList[newMusicKey];
                replaceMusic(collection);
            }
            else if (inSerializer->serializeValue(L"prefer", newMusicKey))
            {
                const musicCollection *collection = (musicCollection *)globalSoundCollectionList[newMusicKey];
                updateMusic(collection);
            }
        }

        //std::vector<char> frameData;
        //serializeNBTValue(*inSerializer, "frame", frameData);
        //imemstream str(&*frameData.begin(), frameData.size());
        //auto iFace = createStreamInterface(str);
        //streamSerializer frameSerializer = streamSerializer(iFace, false, std::endian::little);

        //read bytes directly from the stream
        decoder.addFrameDiff(streamS);

        if (decoder.totalTexture.size == renderTarget.size)
        {
            copyAndCast(decoder.totalTexture.begin(), decoder.totalTexture.end(),
                        renderTarget.baseArray);
            // std::copy(decompressedScreen.begin(), decompressedScreen.end(),
            //           (byte *)renderTarget.baseArray);
        }
        else
        {
            texture oldSizeTex(decoder.totalTexture.size, false);
            copyAndCast(decoder.totalTexture.begin(), decoder.totalTexture.end(),
                        oldSizeTex.baseArray);
            // std::copy(decompressedScreen.begin(), decompressedScreen.end(),
            //           (byte *)oldSizeTex.baseArray);
            fillTransformedTexture(crectangle2(renderTarget.getClientRect()), oldSizeTex,
                                   renderTarget);
        }

        constexpr bool debugMotionVectors = false;
        // for video compression debugging
        if constexpr (debugMotionVectors)
        {
            if (currentServer->clients[0])
            {
                currentServer->clients[0]->encoder.visualize(renderTarget);
            }
        }
        // decoder.visualize(renderTarget);
    }
    // while (selector.wait(sf::microseconds(1))); // pop off all packets on the chain and catch up
    if (status != sf::TcpSocket::Status::Done)
    {
        //auto f = std::bind(&client::addEvent, this, std::placeholders::_1);
        // std::function{f}.target_type();
        currentApplication->listener.unhook(&client::addEvent, this);
        // VERY IMPORTANT: FIRST REMOVE THE SOCKET, THEN DISCONNECT!
        selector.remove(*s.socket);
        // disconnect, in case of any error
        s.socket->disconnect();
        parent->switchVisibleChild(currentMainMenu);
        if (currentServer)
        {
            currentServer->stop();
            currentWorldSelector->refresh();
        }
    }

    delete inSerializer;
    delete inCompound;
}

void client::addEvent(const sf::Event &e)
{
    currentInput.eventHistory.push_back(e);
}

void client::retrievePacketsAsync()
{
    setCurrentThreadName(L"packet retriever");
    auto resetPacketWaiter = [this]()
    {
        packetWaiter->set_value();
        packetWaiter = std::make_shared<std::promise<void>>();
    };
    while (selector.wait())
    {
        std::shared_ptr<sf::Packet> newPacket = std::make_shared<sf::Packet>();
        if ((status = s.socket->receive(*newPacket)))
        {
            resetPacketWaiter();
            break;
        }
        receivedPacketsMutex.lock();
        receivedPackets.push(newPacket);
        receivedPacketsMutex.unlock();
        resetPacketWaiter();
    }
}
