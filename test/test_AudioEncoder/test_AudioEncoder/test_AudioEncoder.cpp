#include <ryulib/AudioIO.hpp>
#include <ryulib/AudioEncoder.hpp>

int main(void) {
	Audio::init();

	AudioEncoder encoder(1, 48000);
	encoder.setOnEncode([&](const void* obj, const void* data, int size) {
		printf("Encoded size: %d \n", size);
	});

	AudioInput audio_input(1, 48000, 4, 3840);
	audio_input.setOnData([&](const void* obj, const void* buffer, int buffer_size) {
		printf("original size: %d \n", buffer_size);
		encoder.add(buffer, buffer_size);
	});
	audio_input.open();

	while (audio_input.isActive()) {
		Pa_Sleep(1000);
	}

	audio_input.close();
}