#include "Server.h"

using namespace std;
using namespace std::chrono_literals;

ENetAddress address;

Server::Server(bool isClientRunning)
	: m_isServerRunning(isClientRunning)
{
	m_server = nullptr;
}

Server::~Server()
{
	if (m_server != nullptr)
	{
		enet_host_destroy(m_server);
	}
}

bool Server::CreateServer(ENetAddress* address)
{
	address->host = ENET_HOST_ANY;
	address->port = 1234;

	m_server = enet_host_create(address /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);

	return m_server != nullptr;
}

void Server::ConnectClient(ENetEvent event, ENetAddress address)
{
	cout << "A new client connected from "
		<< event.peer->address.host
		<< ":" << event.peer->address.port
		<< endl;

	/* Store any relevant client information here. */
	event.peer->data = (void*)(" Client information");

	{
		/* Create a reliable packet of size 7 containing "packet\0" */
		ENetPacket* packet = enet_packet_create("hello",
			strlen("hello") + 1,
			ENET_PACKET_FLAG_RELIABLE);

		enet_host_broadcast(m_server, 0, packet);

		enet_host_flush(m_server);
	}
}

void Server::RecieveMessage(ENetEvent event)
{
	cout << "A packet of length "
		<< event.packet->dataLength << endl
		<< "containing " << (char*)event.packet->data
		<< endl;

	// Display message to clients
	{
		/* Create a reliable packet */
		ENetPacket* packet = enet_packet_create(event.packet->data,
			strlen((char*)event.packet->data) + 1,
			ENET_PACKET_FLAG_RELIABLE);

		enet_host_broadcast(m_server, 0, packet);

		enet_host_flush(m_server);
	}

	// Clean up the packet now that we're done using it
	enet_packet_destroy(event.packet);

}

void Server::DisconnectClient(ENetEvent event)
{
	cout << (char*)event.peer->data << " disconnected."
		<< endl;

	// Reset the peer's client information
	event.peer->data = NULL;
}

void Server::RunServerThread()
{
	// INPUT LOOP START
	ENetEvent event;
	while (m_isServerRunning)
	{
		while (enet_host_service(m_server, &event, 5000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				ConnectClient(event, address);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				RecieveMessage(event);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				DisconnectClient(event);
				SetIsClientRunning();
				break;
			}
		}
	}
	// INPUT LOOP END
}

int main(int argc, char** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, 
			"An error occurred while initiliazing ENet!\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	Server server = Server(true);

	if (!server.CreateServer(&address))
	{
		fprintf(stderr, 
			"An error occurred while trying to create an ENet server.\n");
		return EXIT_FAILURE;
	}

	// thread to have server listen for events
	thread serverThread(&Server::RunServerThread, server);

	serverThread.join();

	return EXIT_SUCCESS;
}