#include <stdio.h>
#include <stdlib.h>
#include <ryulib/AudioIO.hpp>

int main(void) {
	Audio::init();

	AudioInput audio_input(2, SAMPLE_RATE);
	audio_input.setOnData([](const void* obj, const void* data, int size) {
		printf("buffer_size: %d \n", size);
	});
	audio_input.open();

	while (audio_input.is_active()) {
		Pa_Sleep(1000);
	}

	audio_input.close();
}