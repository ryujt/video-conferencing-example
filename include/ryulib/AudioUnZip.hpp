#ifndef AUDIOUNZIP_HPP
#define AUDIOUNZIP_HPP


#include <ryulib/AudioZipUtils.hpp>
#include <ryulib/AudioIO.hpp>
#include <ryulib/AudioDecoder.hpp>
#include <ryulib/debug_tools.hpp>

using namespace std;

class AudioUnZip {
public:
	AudioUnZip(int channels, int sampe_rate)
		: audio_output_(channels, sampe_rate), audio_decoder_(channels, sampe_rate)
	{
		audio_output_.setOnError(
			[&](const void* obj, int error_code) {
				if (OnError_ != nullptr) OnError_(this, error_code);
			}
		);
		audio_output_.open();

		audio_decoder_.setOnError(
			[&](const void* obj, int error_code) {
				if (OnError_ != nullptr) OnError_(this, error_code);
			}
		);
		audio_decoder_.setOnDecode(
			[&](const void* obj, const void* data, int size) {
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