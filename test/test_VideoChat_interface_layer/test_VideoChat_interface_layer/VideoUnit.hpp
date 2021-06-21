#pragma once

#include <functional>

using namespace std;

typedef function<void(const void*, int)> VideoDataEvent;

class VideoUnit {
public:
	void start()
	{

	}

	void stop()
	{

	}

	void play(const void* data, int size)
	{

	}

	void setOnData(VideoDataEvent event) { onData = event; }

private:
	VideoDataEvent onData = nullptr;
};