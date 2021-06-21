#pragma once

#include <ryulib/SocketClient.hpp>
#include <string>

using namespace std;

typedef function<void(int, const void*, int)> ReceivedEvent;

class SocketUnit {
public:
	void connect(string host, int port)
	{

	}

	void disconnect()
	{

	}

	void sendVideoData(const void* data, int size)
	{

	}

	void sendAudioData(const void* data, int size)
	{

	}

	void setOnReceived(ReceivedEvent event) { onReceived = event;  }

private:
	ReceivedEvent onReceived = nullptr;
};