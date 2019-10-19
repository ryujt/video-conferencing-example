#ifndef AUDIOZIP_HPP
#define AUDIOZIP_HPP


#include <ryulib/base.hpp>
#include <ryulib/AudioZipUtils.hpp>
#include <ryulib/AudioIO.hpp>
#include <ryulib/AudioEncoder.hpp>
#include <ryulib/debug_tools.hpp>

using namespace std;

class AudioZip {
public:
	AudioZip()
		: audio_input_(CHANNEL, SAMPLE_RATE)
	{
		audio_encoder_.setOnError(
			[&](int error_code) {
				if (OnError_ != nullptr) OnError_(error_code);
			}
		);

		audio_input_.setOnError(
			[&](int error_code) {
				if (OnError_ != nullptr) OnError_(error_code);
			}
		);
		audio_input_.setOnData(
			[&](const void* buffer, int buffer_size) {
				//printf("buffer_size: %d \n", buffer_size);
				audio_encoder_.add(buffer, buffer_size);
			}
		);
	}

	void start() 
	{
		audio_input_.open();
	}

	void stop() 
	{
		audio_input_.close();
	}

	void setOnError(IntegerEvent event) { OnError_ = event; }
	void setOnEncode(DataEvent event) { audio_encoder_.setOnEncode(event); }

private:
	AudioInput audio_input_; 
	AudioEncoder audio_encoder_;
	IntegerEvent OnError_ = nullptr;
};


#endif  // AUDIOZIP_HPP