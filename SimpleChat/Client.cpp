#include "Client.h"

using namespace std;

ENetAddress address;
ENetPeer* peer; /* server connecting to */

Client::Client(int id, string username)
	: m_id(id)
	, m_username(username)
	, m_isRunning(true)
{
	m_client = nullptr;
}

Client::~Client()
{
	//if (m_client != nullptr)
	//{
	//	m_client = nullptr;
	//}
}

bool Client::CreateClient()
{
	m_client = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);

	return m_client != nullptr;
}

void Client::ConnectToServer()
{
	/* Initialize address to connect to */
	ENetEvent event;
	enet_address_set_host(&address, "127.0.0.1");
	address.port = 1234;

	// Initialize the peer and connect to peer
	peer = enet_host_connect(GetClient(), &address, 2, 0);

	if (peer == nullptr)
	{
		fprintf(stderr,
			"No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}

	// Check if server contacted client back
	if (enet_host_service(GetClient(), &event, 5000) > 0
		&& event.type == ENET_EVENT_TYPE_CONNECT)
	{
		cout << "Connection to 127.0.0.1:1234 succeeded." << endl;
	}
	else
	{
		enet_peer_reset(peer);
		cout << "Connection to 127.0.0.1:1234 failed." << endl;
	}

	//SendPacket(peer, m_username.c_str());
}

/*
Sends a packet with data to a peer.
*/
void Client::SendPacket(ENetPeer* peer, const char* data, int channelID)
{
	ENetPacket* packet = enet_packet_create(data, 
		strlen(data) + 1, 
		ENET_PACKET_FLAG_RELIABLE);

	// Send the packet to the peer on the chane
	enet_peer_send(peer, channelID, packet);
	enet_host_flush(m_client);
}

/*
Check to see if client recieves any messages
*/
void Client::MessageLoop()
{
	// Check for messages while user hasn't quit chat
	while (m_isRunning)
	{
		ENetEvent event;
		while (enet_host_service(m_client, &event, 1000) > 0) {
			switch (event.type)
			{
			    case ENET_EVENT_TYPE_RECEIVE:
			    {
				    //Display the message received from the server
				    /* Create a reliable packet */
				  //  ENetPacket* packet = enet_packet_create(
						//event.packet->data,
					 //   strlen((char*)event.packet->data) + 1,
					 //   ENET_PACKET_FLAG_RELIABLE);

				  //  enet_host_broadcast(m_client, 0, packet);

					cout << event.packet->data << endl;

			        /* Clean up the packet now that we're done using it. */
					enet_packet_destroy(event.packet);

				    //enet_host_flush(m_client);
			    }
			}
		}
	}
}

void Client::InputLoop(ENetPeer* peer)
{
	string msg = "";
	cin.ignore();
	while (GetIsRunning())
	{
		// Get user's message
		// may have to move line outside while loop
		std::getline(cin, msg);

		// Check if user typed 'quit' to quit the chat
		if (msg == "quit")
		{
			SetIsRunning();
		}

		// TODO: Display the message
		msg = m_username + ": " + msg;
		SendPacket(peer, msg.c_str());

	}
}

int main(int argc, char** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initiliazing ENet!\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	string userName = "User";

	// Ask user to input their name
	cout << "Weclome to LUUM! Please enter your name:\n";
	cin >> userName;

	// Create the client
	Client client = Client(1, userName);

	if (!client.CreateClient())
	{
		fprintf(stderr,
			"An error occurred while tyring to create an ENet client host\n");
		return EXIT_FAILURE;
	}

	// TODO: move this to server so it announces when a new user joins
	cout << userName << " has joined.\n";

	// Connect the client to the server
	client.ConnectToServer();

	// create thread that waits for messages
	thread checkMsgThread(&Client::MessageLoop, client);

	// create and run thread to check for input
	thread checkInputThread(&Client::InputLoop, client, peer);

	// Tell thread to join back
	checkInputThread.join();
	checkMsgThread.join();

	cout << userName << " has left. Goodbye!\n";

	if (client.GetClient() != nullptr)
	{
		enet_host_destroy(client.GetClient());
	}

	return EXIT_SUCCESS;
}