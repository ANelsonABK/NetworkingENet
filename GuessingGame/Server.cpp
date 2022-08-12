#include "Server.h"
#include <atomic>

std::atomic<bool> isRunning(false);

using namespace std;

Server::Server(int numConnections)
{
	_maxConnections = numConnections;
	_currConnections = 0;
	_server = nullptr;
	_randomNumber = 0;
}

Server::~Server()
{
}

/* Attempt to create a server for the guessing game. */
bool Server::CreateServer()
{
	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = 1234;

	_server = enet_host_create(&address, _maxConnections, 2, 0, 0);

	return _server != nullptr;
}

void Server::ServerProcessPackets()
{
	// Process packets while server is alive
	while (isRunning.load())
	{
		ENetEvent event;
		while (enet_host_service(_server, &event, 1000) > 0)
		{
			switch (event.type)
			{
			    case ENET_EVENT_TYPE_CONNECT:
			    {
				    // TODO: let client know connection was successful
			    }
			    case ENET_EVENT_TYPE_RECEIVE:
			    {
				    HandleReceivePacket(event);
				    break;
			    }
			    case ENET_EVENT_TYPE_DISCONNECT:
			    {
				    // Disconnect clients from server
				    DisconnectClient(event);
			    }
			}
		}
	}
}

/* Attempt to connect a client to the server */
void Server::ConnectClient(ENetEvent event)
{
	if (_currConnections == _maxConnections)
	{
		// TODO: Tell client it can't connect
	}
	else
	{
		cout << "A new client connected from "
			<< event.peer->address.host
			<< ":" << event.peer->address.port
			<< endl;
		_currConnections++;
		event.peer->data = (void*)("Client Information");

		// TODO: Broadcast a welcome player
	}
}

/* Broadcast that a new player joined */
void Server::BroadcastPlayerPacket()
{
	PlayerPacket* playerPacket = new PlayerPacket();
	playerPacket->playerId = _currConnections;
	playerPacket->message = "Welcome Player " + _currConnections;

	ENetPacket* packet = enet_packet_create(playerPacket,
		sizeof(*playerPacket),
		ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast(_server, 0, packet);

	enet_host_flush(_server);
	delete playerPacket;
}

/* Broadcast if a player guess correctly */
void Server::BroadcastIsCorrectPacket()
{
	IsCorrectPacket* correctPacket = new IsCorrectPacket();

	// TODO: Broadcast the result

	enet_host_flush(_server);
	delete correctPacket;
}

void Server::HandleReceivePacket(const ENetEvent& event)
{
	GamePacket* GuessGamePacket = (GamePacket*)(event.packet->data);

	if (GuessGamePacket->Type == PHT_IsCorrect)
	{
		IsCorrectPacket* CheckGuessPacket = (IsCorrectPacket*)(event.packet->data);
		if (CheckGuessPacket)
		{
			// TODO: check if a player is correct

		}
	}
	else
	{
		std::cout << "Invalid Packet.\n";
	}

	// Clean up the packet
	enet_packet_destroy(event.packet);
	{
		enet_host_flush(_server);
	}
}

/* Handle when a client disconnects */
void Server::DisconnectClient(ENetEvent event)
{
	_currConnections--;
	cout << (char*)event.peer->data << " disconnected.\n";

	/* Reset the peer's client information */
	event.peer->data = NULL;

	// TODO: notify remaining player game ended

	// Close the server
	isRunning.store(false);
}

/* Start the game and broadcast to players that it has started. */
void Server::StartGame()
{
	SetRandomNumber();

	// TODO: broadcast to players
}