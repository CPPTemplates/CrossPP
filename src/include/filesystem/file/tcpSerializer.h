#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <utility>
#include "optimization/optimization.h"
#include <future>
#include <functional>
#include "include/filesystem/serializer.h"
#include <SFML/Network/Packet.hpp>

struct tcpSerializer : streamBaseInterface
{
	sf::TcpSocket *socket = nullptr;
	sf::Packet *receivingPacket = new sf::Packet();
	sf::Packet *sendingPacket = new sf::Packet();
	const char *receivingPacketPos = nullptr;
	const char *receivingPacketEndPos = nullptr;
	std::thread *packetSender = nullptr;

	inline bool write(const char *const &value, const size_t &size) override
	{
		sendingPacket->append(value, size);
		return true;
	}
	inline bool read(char *const &value, const size_t &size) override
	{
		const auto newDataPos = receivingPacketPos + size;
		assumeInRelease(newDataPos <= receivingPacketEndPos);
		std::copy(receivingPacketPos, newDataPos, value);
		receivingPacketPos = newDataPos;
		return true;
	}
	inline sf::Socket::Status sendPacket()
	{
		// this way, other threads can construct a new packet already while the old packet is sending
		sf::Packet *packetCopy = sendingPacket;
		sendingPacket = new sf::Packet();
		// std::swap(packetCopy, sendingPacket);
		// sendingPacket;
		if (packetSender)
		{
			packetSender->join();
			delete packetSender;
		}
		packetSender = new std::thread([this, packetCopy]
									   {
					if (packetCopy->getDataSize() == 0)
					{
						throw "sending empty packet!!";
					}
					//const sf::Socket::Status &status = 
					socket->send(*packetCopy);
					delete packetCopy; });

		return sf::Socket::Done;
	}

	inline void setReceivingPacket(sf::Packet *p)
	{
		receivingPacket = p;
		receivingPacketPos = (const char *)p->getData();
		receivingPacketEndPos = receivingPacketPos + p->getDataSize();
	}
	inline sf::Socket::Status receivePacket()
	{
		const sf::Socket::Status &status = socket->receive(*receivingPacket);
		setReceivingPacket(receivingPacket);
		return status;
	}

	~tcpSerializer()
	{
		delete receivingPacket;
		delete sendingPacket;
	}
	// inline bool write(const char* const& value, const size_t& size) const
	//{
	//	size_t sentSize;
	//	if (socket->send(value, size, sentSize) == sf::Socket::Status::Done) {
	//		if (isDebugging && sentSize != size)
	//		{
	//			throw "received different data";
	//		}
	//		return true;
	//	}
	//	return false;
	// }
	// inline bool read(char* const& value, const size_t& size) const
	//{
	//	size_t receivedSize;
	//	if (socket->receive(value, size, receivedSize) == sf::Socket::Status::Done) {
	//		if (isDebugging && receivedSize != size)
	//		{
	//			throw "received different data";
	//		}
	//		return true;
	//	}
	//	return false;
	// }
};