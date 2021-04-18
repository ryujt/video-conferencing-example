#include <ryulib/AudioIO.hpp>

int main(void) {
	Audio::init();

	AudioInput audio_input(1, 48000, 4, 3840);
	audio_input.setOnData([](const void* obj, const void* data, int size) {
		printf("buffer_size: %d \n", size);
	});
	audio_input.open();

	while (audio_input.isActive()) {
		Pa_Sleep(1000);
	}

	audio_input.close();
}