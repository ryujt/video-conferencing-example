#include <iostream>
#include <string>
#include <ryulib/SocketClient.hpp>
#include <ryulib/AudioZip.hpp>
#include <ryulib/AudioUnZip.hpp>

using namespace std;

int main()
{
	Audio::init();

	SocketClient socket;

	AudioUnZip unzip(1, 48000);
	unzip.setOnError([&](const void* obj, int error_code) {
		printf("AudioUnZip - error: %d", error_code);
	});

	AudioZip zip(1, 48000);
	zip.setOnError([&](const void* obj, int error_code) {
		printf("AudioZip - error: %d", error_code);
	});
	zip.setOnEncode([&](const void* obj, const void* data, int size) {
		socket.send(0, data, size);
	});

	socket.setOnConnected([&]() {
		printf("Connected! \n");
	});

	socket.setOnDisconnected([]() {
		printf("Disconnected! \n");
	});

	socket.setOnError([](int code, string msg) {
		printf("Error - %d, %s \n", code, msg.c_str());
	});

	socket.setOnReceived([&](Packet* packet) {
		if (packet->getDataSize() > 0) {
			unzip.play(packet->getData(), packet->getDataSize());
		}
	});

	socket.connect("3.34.52.188", 1001);

	while (true) {
		string line;
		getline(cin, line);

		if (line == "s") zip.start();
	}
}
