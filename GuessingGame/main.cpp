#include "Server.h"
#include "Client.h"

using namespace std;

/* Runs either a server or client */
int main(int argc, char** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr,
			"An error occurred while initializing ENet.\n");
		cout << "An error occurred while initializing ENet.\n";
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	cout << "Please make a selection: \n";
	cout << "1) Create Server\n";
	cout << "2) Create Client\n";
	cout << "3) Exit\n";

	int userInput;
	cin >> userInput;
	if (userInput == 1)
	{
		// Create a server with two clients
		Server server = Server(2);
		if (!server.CreateServer())
		{
			fprintf(stderr,
				"An error occurred while initializing ENet.\n");
			return EXIT_FAILURE;
		}

		// wait for max players to join
		cout << "Waiting for players to join...\n";
		thread waitThread(&Server::WaitForPlayers, server);
		waitThread.join();

		// Launch the game
		server.StartGame();
		thread serverThread(&Server::ServerProcessPackets, server);

		serverThread.join();

		if (server.GetServer() != nullptr)
		{
			enet_host_destroy(server.GetServer());
		}
	}
	else if (userInput == 2)
	{

		// Create a client
		Client client = Client();
		if (!client.CreateClient())
		{
			fprintf(stderr,
				"An error occurred while initializing ENet.\n");
			return EXIT_FAILURE;
		}

		// Attempt to connect to server
		if (!client.ConnectToServer())
		{
			fprintf(stderr,
				"No available peers for initiating an ENet connection.\n");
			return EXIT_FAILURE;
		}

		// Start threads for packets and input
		thread inputThread(&Client::InputLoop, client);
		thread packetThred(&Client::ClientProcessPackets, client);

		inputThread.join();
		packetThred.join();

		if (client.GetClient() != nullptr)
		{
			enet_host_destroy(client.GetClient());
		}
	}
	else if (userInput == 3)
	{
		cout << "Goodbye.\n";
	}
	else
	{
		cout << "Invalid input.\n";
	}

	return EXIT_SUCCESS;
}