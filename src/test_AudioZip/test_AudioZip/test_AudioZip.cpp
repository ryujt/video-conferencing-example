#include <stdio.h>
#include <stdlib.h>
#include <ryulib/AudioZip.hpp>
#include <ryulib/AudioUnZip.hpp>

int main(void) {
	Audio::init();

	AudioUnZip unzip(1, SAMPLE_RATE);
	unzip.setOnError([&](const void* obj, int error_code) {
		printf("AudioUnZip - error: %d", error_code);
	});

	AudioZip zip(1, SAMPLE_RATE);
	zip.setOnError([&](const void* obj, int error_code) {
		printf("AudioZip - error: %d", error_code);
	});
	zip.setOnEncode([&](const void* obj, const void* data, int size) {
		unzip.play(data, size);
	});

	zip.start();

	while (true) {
		Pa_Sleep(1000);
	}
}