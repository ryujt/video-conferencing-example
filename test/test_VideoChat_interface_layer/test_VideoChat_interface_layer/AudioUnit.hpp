#pragma once

#include <ryulib/AudioZip.hpp>
#include <ryulib/AudioUnZip.hpp>
#include <functional>

using namespace std;

typedef function<void(const void*, int)> AudioDataEvent;

const int AUDIO_CHANNELS = 1;
const int AUDIO_SAMPLE_RATE = 48000;

class AudioUnit {
public:
	AudioUnit()
	{
		Audio::init();

		audioZip = new AudioZip(AUDIO_CHANNELS, AUDIO_SAMPLE_RATE);
		audioZip->setOnEncode([&](const void* obj, const void* data, int size) {
			if (onData != nullptr) onData(data, size);
		});

		audioUnzip = new AudioUnZip(AUDIO_CHANNELS, AUDIO_SAMPLE_RATE);
	}

	void start()
	{
		audioZip->start();
	}

	void stop()
	{
		audioZip->stop();
	}

	void play(const void* data, int size)
	{
		audioUnzip->play(data, size);
	}

	void setOnData(AudioDataEvent event) { onData = event;  }

private:
	AudioZip* audioZip = nullptr;
	AudioUnZip* audioUnzip = nullptr;
	AudioDataEvent onData = nullptr;
};