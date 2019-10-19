#ifndef AUDIOIO_HPP
#define AUDIOIO_HPP


#include <ryulib/AudioZipUtils.hpp>
#include <portaudio.h>
#include <ryulib/base.hpp>
#include <ryulib/ThreadQueue.hpp>
#include <ryulib/debug_tools.hpp>

using namespace std;

class Audio {
public:
	static int init()
	{
		PaError err = paNoError;

		err = Pa_Initialize();
		if (err != paNoError) {
			DebugOutput::trace("Error: Pa_Initialize. \n");
			return false;
		}

		return true;
	}
};

class AudioInput {
public:
	AudioInput(int channels, int sampe_rate)
		: channels_(channels), sampe_rate_(sampe_rate), buffer_size_(SAMPLE_SIZE * FRAMES_PER_BUFFER * channels) 
	{
	}

	int open() 
	{
		PaError err = paNoError;
		PaStreamParameters  inputParameters;

		inputParameters.device = Pa_GetDefaultInputDevice();
		if (inputParameters.device == paNoDevice) {
			DebugOutput::trace("Error: No default input device. \n");
			if (OnError_ != nullptr) OnError_(this, ERROR_NO_DEFAULT_INPUT_DEVICE);
			return ERROR_NO_DEFAULT_INPUT_DEVICE;
		}

		inputParameters.channelCount = channels_;
		inputParameters.sampleFormat = paFloat32;
		inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;

		err = Pa_OpenStream(&stream_, &inputParameters, NULL, sampe_rate_, FRAMES_PER_BUFFER, paClipOff, recordCallback, this);
		if (err != paNoError) {
			DebugOutput::trace("Error: AudioInput- Pa_OpenStream \n");
			if (OnError_ != nullptr) OnError_(this, ERROR_OPEN_INPUT_DEVICE);
			return ERROR_OPEN_INPUT_DEVICE;
		}

		err = Pa_StartStream(stream_);
		if (err != paNoError) {
			DebugOutput::trace("Error: AudioInput - Pa_StartStream \n");
			if (OnError_ != nullptr) OnError_(this, ERROR_START_INPUT_DEVICE);
			return ERROR_START_INPUT_DEVICE;
		}

		return 0;
	}

	void close() 
	{
		Pa_CloseStream(stream_);
	}

	bool is_active()
	{
		return Pa_IsStreamActive(stream_) == 1;
	}

	void setOnError(IntegerEvent event) { OnError_ = event; }
	void setOnData(const DataEvent &value) { on_data_ = value; }

private:
	static int recordCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData) 
	{

		AudioInput *audio_input = (AudioInput *) userData;
		if (audio_input->on_data_ != nullptr) audio_input->on_data_(audio_input, inputBuffer, audio_input->buffer_size_);

		return paContinue;
	}

	int channels_;
	int sampe_rate_;
	int buffer_size_;

	PaStream* stream_;
	IntegerEvent OnError_ = nullptr;
	DataEvent on_data_ = nullptr;
};

class AudioOutput {
public:
	AudioOutput(int channels, int sampe_rate)
		: channels_(channels), sampe_rate_(sampe_rate), buffer_size_(SAMPLE_SIZE * FRAMES_PER_BUFFER * channels) 
	{

		DebugOutput::trace("AudioOutput - buffer_size_: %d \n", buffer_size_);

		mute_ = malloc(buffer_size_);
		memset(mute_, 0, buffer_size_);
	}

	int open() 
	{
		PaError err = paNoError;
		PaStreamParameters  outputParameters;

		outputParameters.device = Pa_GetDefaultOutputDevice();
		if (outputParameters.device == paNoDevice) {
			DebugOutput::trace("Error: AudioOutput - paNoDevice \n");
			if (OnError_ != nullptr) OnError_(this, ERROR_NO_DEFAULT_OUTPUT_DEVICE);
			return ERROR_NO_DEFAULT_OUTPUT_DEVICE;
		}
		outputParameters.channelCount = channels_;
		outputParameters.sampleFormat = paFloat32;
		outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;

		err = Pa_OpenStream(&stream_, NULL, &outputParameters, sampe_rate_, FRAMES_PER_BUFFER, paClipOff, playCallback, this);
		if (err != paNoError) {
			DebugOutput::trace("Error: AudioOutput - Pa_OpenStream \n");
			if (OnError_ != nullptr) OnError_(this, ERROR_OPEN_OUTPUT_DEVICE);
			return ERROR_OPEN_OUTPUT_DEVICE;
		}

		err = Pa_StartStream(stream_);
		if (err != paNoError) {
			DebugOutput::trace("Error: AudioOutput - Pa_StartStream \n");
			if (OnError_ != nullptr) OnError_(this, ERROR_START_OUTPUT_DEVICE);
			return ERROR_START_OUTPUT_DEVICE;
		}

		return 0;
	}

	void close()
	{
		Pa_CloseStream(stream_);
	}

	void play(const void *data, int size)
	{
		Memory* memory = new Memory(data, size);
		queue_.push(memory);
	}

	bool is_active() { return Pa_IsStreamActive(stream_) == 1; }
	void setOnError(IntegerEvent event) { OnError_ = event; }

private:
	static int playCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData) 
	{

		AudioOutput *audio_output = (AudioOutput *) userData;

		Memory* memory;
		if (audio_output->queue_.pop(memory)) {
			memcpy(outputBuffer, memory->getData(), memory->getSize());
			delete memory;
		} else {
			memcpy(outputBuffer, audio_output->mute_, audio_output->buffer_size_);
		}

		return paContinue;
	}

	int channels_;
	int sampe_rate_;
	int buffer_size_;
	void *mute_;

	PaStream *stream_;
	ThreadQueue<Memory*> queue_;
	IntegerEvent OnError_ = nullptr;
};


#endif  // AUDIOIO_HPP