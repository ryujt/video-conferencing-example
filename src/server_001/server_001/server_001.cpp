#include <stdio.h>
#include <ryulib/SocketUtils.hpp>
#include <ryulib/SocketServer.hpp>

int main()
{
	SocketServer server;

	server.setOnConnected([](Connection* connection) {
		printf("Connected - ID: %d \n", connection->getID());
	});

	server.setOnDisconnected([](Connection* connection) {
		printf("Disconnected - ID: %d \n", connection->getID());
	});

	server.setOnError([](int code, string msg) {
		printf("Error - %d, %s \n", code, msg.c_str());
	});

	server.setOnReceived([&](Connection* connection, Packet* packet) {
		printf("Received - %d, %s \n", packet->packet_type, packet->getText());
		server.sendToOther(connection, packet);
	});

	server.start(1234);
}
