#include "Client.h"

using namespace std;

Client::Client()
	: _playerId(0)
	, _quitGame(false)
	, _client(nullptr)
	, _peer(nullptr)
{

}

Client::~Client()
{

}

bool Client::CreateClient()
{
	_client = enet_host_create(NULL, 1, 2, 0, 0);

	return _client != nullptr;
}

/* Attempt to connect the client to the server */
bool Client::ConnectToServer()
{
	ENetAddress address;

	// Connect to the server
	enet_address_set_host(&address, "127.0.0.1");
	address.port = 1234;

	/* Initiate a connection */
	_peer = enet_host_connect(_client, &address, 2, 0);

	return _peer != nullptr;
}

void Client::ClientProcessPackets()
{
	// Wait for a response while user is playing and can't answer
	while (!_quitGame.load() && !_canAnswer.load())
	{
		ENetEvent event;
		while (enet_host_service(_client, &event, 1000) > 0)
		{
			switch (event.type)
			{
			    case ENET_EVENT_TYPE_CONNECT:
			    {
					cout << "Connection successful!\n";
					cout << "Welcome to the Guessing Game!\n";
					break;
			    }
				case ENET_EVENT_TYPE_RECEIVE:
				{
					break;
				}
			}
		}
	}
}

/* Handle a packet from the server */
void Client::HandleReceivePacket(ENetEvent& event)
{
	GamePacket* RecGamePacket = (GamePacket*)(event.packet->data);
	if (RecGamePacket)
	{
		if (RecGamePacket->Type == PHT_IsCorrect)
		{
			IsCorrectPacket* CorrectGamePacket = (IsCorrectPacket*)
				(event.packet->data);
			if (CorrectGamePacket)
			{
				// Check if the player scored correctly
				if (CorrectGamePacket->winnerId != -1)
				{
					if (CorrectGamePacket->isCorrect)
					{
						cout << "Congralutations! You guess correct!\n";
					}
					else
					{
						cout << "Player " << CorrectGamePacket->playerId
							 << " is the winner.\n";
						cout << "The correct number was "
							 << CorrectGamePacket->correctNumber << endl;
					}

					// Stop the game
					_quitGame.store(true);
				}
				else 
				{
					// Let the player guess again
					cout << "Incorrect. Try again: \n";
					_canAnswer.store(true);
				}
			}
		}
	}
	else
	{
		cout << "Invalid Packet \n";
	}

	/* Clean up the packet */
	enet_packet_destroy(event.packet);
	{
		enet_host_flush(_client);
	}
}

/* Waits for the user to make an input */
void Client::InputLoop()
{
	string response = "";
	cin.ignore();

	/* While player is playing and has not answered */
	while (!_quitGame.load() && _canAnswer.load())
	{
		// Get a response from the user
		std::getline(cin, response);

		if (response == "quit")
		{
			_quitGame.store(true);

			// TODO: Send to server that a player quit
		}
		else
		{
			// Pack response and send it to server
			_canAnswer.store(false);
		}
	}
}

/* Send a packet with the player's guess to the server */
void Client::SendPacket(string guess)
{
	PlayerGuessPacket* GuessPacket = new PlayerGuessPacket();
	GuessPacket->playerId = _playerId;
	GuessPacket->playerGuess = atoi(guess.c_str());

	ENetPacket* packet = enet_packet_create(
		GuessPacket,
		sizeof(*GuessPacket),
		ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(_peer, 1, packet);
	enet_host_flush(_client);
	delete GuessPacket;
}