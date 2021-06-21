#pragma once

#include <functional>

using namespace std;

typedef function<void(const void*, int)> AudioDataEvent;

class AudioUnit {
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

	void setOnData(AudioDataEvent event) { onData = event;  }

private:
	AudioDataEvent onData = nullptr;
};