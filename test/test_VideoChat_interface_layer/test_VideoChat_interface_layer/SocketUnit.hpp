#pragma once

#include "PacketType.hpp"
#include <ryulib/SocketClient.hpp>
#include <string>
#include <functional>

using namespace std;

typedef function<void(int, const void*, int)> ReceivedEvent;

class SocketUnit {
public:
	SocketUnit()
	{
		socket.setOnReceived([&](Packet* packet) {
			if (onReceived != nullptr) onReceived(packet->packet_type, packet->getData(), packet->getDataSize());
		});
	}

	void connect(string host, int port)
	{
		socket.connect(host, port);
	}

	void disconnect()
	{
		socket.disconnect();
	}

	void sendVideoData(const void* data, int size)
	{
		socket.send(PacketType::VideoPacket, data, size);
	}

	void sendAudioData(const void* data, int size)
	{
		socket.send(PacketType::AudioPacket, data, size);
	}

	void setOnReceived(ReceivedEvent event) { onReceived = event;  }

private:
	SocketClient socket;
	ReceivedEvent onReceived = nullptr;
};