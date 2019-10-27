#include <stdio.h>
#include <stdlib.h>
#include <ryulib/AudioIO.hpp>

int main()
{
	Audio::init();

	AudioOutput audio_output(2, 48000, 3840);
	audio_output.open();

	AudioInput audio_input(2, 48000, 3840);
	audio_input.setOnData([&](const void* obj, const void* data, int size) {
		audio_output.play(data, size);
	});
	audio_input.open();

	while (audio_input.isActive()) {
		Pa_Sleep(1000);
	}

	audio_input.close();
}
