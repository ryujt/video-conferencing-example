#pragma once

#include "PacketType.hpp"
#include "SocketUnit.hpp"
#include "VideoUnit.hpp"
#include "AudioUnit.hpp"
#include <string>

using namespace std;

class VideoChat {
public:
	VideoChat()
	{
		socketUnit.setOnReceived([&](int packekType, const void* data, int size) {
			switch (packekType) {
				case PacketType::PACKET_TYPE_VIDEO: videoUnit.play(data, size); break;
				case PacketType::PACKET_TYPE_AUDIO: audioUnit.play(data, size); break;
			}
		});

		videoUnit.setOnData([&](const void* sender, const void* data, int size) {
			socketUnit.sendVideoData(data, size);
		});

		audioUnit.setOnData([&](const void* sender, const void* data, int size) {
			socketUnit.sendAudioData(data, size);
		});
	}

	void connect(string host, int port)
	{
		socketUnit.connect(host, port);
	}

	void disconnect()
	{
		socketUnit.disconnect();
	}

	void start()
	{
		videoUnit.start();
		audioUnit.start();
	}

	void stop()
	{
		videoUnit.stop();
		audioUnit.stop();
	}

private:
	SocketUnit socketUnit;
	VideoUnit videoUnit;
	AudioUnit audioUnit;
};