#include <stdio.h>
#include <stdlib.h>
#include <ryulib/AudioIO.hpp>

int main()
{
	Audio::init();

	AudioOutput audio_output(2, SAMPLE_RATE);
	audio_output.open();

	AudioInput audio_input(2, SAMPLE_RATE);
	audio_input.setOnData([&](const void* obj, const void* buffer, int buffer_size) {
		audio_output.play(buffer, buffer_size);
	});
	audio_input.open();

	while (audio_input.is_active()) {
		Pa_Sleep(1000);
	}

	audio_input.close();
}
