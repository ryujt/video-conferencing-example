#include "VideoChat.hpp"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    VideoChat videoChat;
    videoChat.connect("127.0.0.1", 1234);

	while (true) {
		string line;
		getline(cin, line);

		if (line == "s") videoChat.start();
		if (line == "t") videoChat.stop();
	}
}