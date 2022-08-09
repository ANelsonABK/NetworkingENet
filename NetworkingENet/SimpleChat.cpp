#pragma once
#include <enet/enet.h>
#include <iostream>

using namespace std;

ENetAddress address;
ENetHost* server = nullptr;
ENetHost* client = nullptr;

bool CreateServer()
{
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 1234;
	server = enet_host_create(&address /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);

	return server != nullptr;
}

// ex: thread eventThread = thread(StartListeningForEvents, server);

int main(int argc, char** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		cout << "An error occurred while initializing ENet." << endl;
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	cout << "1) Create Server " << endl;
	cout << "2) Create Client " << endl;
	int UserInput;
	cin >> UserInput;



	return EXIT_SUCCESS;
}