#pragma once
#include "Packets.h"
#include "enet/enet.h"
#include <iostream>

class Server
{
	int _maxConnections;
	int _currConnections;
	int _randomNumber;

public:
	Server(int numConnections = 32);
	~Server();

	// Functions
	bool CreateServer();
	void ServerProcessPackets();
	void DisconnectClient(ENetEvent event);
	void ConnectClient(ENetEvent event);

	void BroadcastIsCorrectPacket();
	void BroadcastPlayerPacket();
	void HandleReceivePacket(const ENetEvent& event);
	void StartGame();

	void SetRandomNumber() { _randomNumber = (1 + rand() % 100); }
	int GetRandomNumber() { return _randomNumber; }
	ENetHost* GetServer() { return _server; }

	int GetCurrConnections() { return _currConnections; }
	int GetMaxConnections() { return _maxConnections; }

	void WaitForPlayers();

protected:
	ENetHost* _server;
};