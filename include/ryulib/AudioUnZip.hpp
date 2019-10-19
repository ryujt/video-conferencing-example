#ifndef AUDIOUNZIP_HPP
#define AUDIOUNZIP_HPP


#include <ryulib/AudioZipUtils.hpp>
#include <ryulib/AudioIO.hpp>
#include <ryulib/AudioDecoder.hpp>
#include <ryulib/debug_tools.hpp>

using namespace std;

class AudioUnZip {
public:
	AudioUnZip()
		: audio_output_(CHANNEL, SAMPLE_RATE)
	{
		audio_output_.setOnError(
			[&](int error_code) {
				if (OnError_ != nullptr) OnError_(error_code);
			}
		);
		audio_output_.open();

		audio_decoder_.setOnError(
			[&](int error_code) {
				if (OnError_ != nullptr) OnError_(error_code);
			}
		);
		audio_decoder_.setOnDecode(
			[&](const void* data, int size) {
				//printf("size: %d \n", size);
				audio_output_.play(data, size);
			}
		);
	}

	~AudioUnZip()
	{
		audio_output_.close();
	}

	void play(const void* data, int size) 
	{
		audio_decoder_.play(data, size);
	}

	void setOnError(IntegerEvent event) { OnError_ = event; }

private:
	AudioOutput audio_output_; 
	AudioDecoder audio_decoder_;
	IntegerEvent OnError_ = nullptr;
};


#endif  // AUDIOUNZIP_HPP