#ifndef AUDIOENCODER_HPP
#define AUDIOENCODER_HPP


#include <ryulib/AudioZipUtils.hpp>
#include <ryulib/base.hpp>
#include <ryulib/SimpleThread.hpp>
#include <ryulib/SuspensionQueue.hpp>
#include <opus/opus.h>
#include <ryulib/debug_tools.hpp>

using namespace std;

class AudioEncoder {
public:
	AudioEncoder(int channels, int sampe_rate)
	{
		int errorCode;

		opus_ = opus_encoder_create(sampe_rate, channels, OPUS_APPLICATION_AUDIO, &errorCode);
		if (errorCode < 0) {
			DebugOutput::trace("Error - opus_encoder_create (%d)\n", ERROR_OPEN_ENCODER);
			if (OnError_ != nullptr) OnError_(this, ERROR_OPEN_ENCODER);
		}

		errorCode = opus_encoder_ctl(opus_, OPUS_SET_BITRATE(BITRATE));
		if (errorCode < 0) {
			opus_encoder_destroy(opus_);
			DebugOutput::trace("Error - opus_encoder_ctl (%d)\n", ERROR_OPEN_ENCODER);
			if (OnError_ != nullptr) OnError_(this, ERROR_OPEN_ENCODER);
		}

		queue_ = new SuspensionQueue<Memory*>();
		thread_ = new SimpleThread(thread_OnExecute);
	}

	void add(const void* data, int size) 
	{
		Memory* memory = new Memory(data, size);
		queue_->push(memory);
	}

	void setOnError(IntegerEvent event) { OnError_ = event; }
	void setOnEncode(DataEvent event) { OnEncode_ = event; }

private:
	SimpleThread* thread_;
	SimpleThreadEvent thread_OnExecute = [&](SimpleThread * thread) {
		while (thread->isTerminated() == false) {
			Memory* data = queue_->pop();

			char buffer[FRAMES_PER_BUFFER * SAMPLE_SIZE * CHANNEL];
			int size_out = opus_encode_float(opus_, (float*) data->getData(), FRAMES_PER_BUFFER, (unsigned char*) buffer, sizeof(buffer));
			if (OnEncode_ != nullptr) OnEncode_(this, buffer, size_out);

			delete data;
		}
	};

private:
	SuspensionQueue<Memory*>* queue_;
	OpusEncoder* opus_;
	IntegerEvent OnError_ = nullptr;
	DataEvent OnEncode_ = nullptr;
};


#endif  // AUDIOENCODER_HPP