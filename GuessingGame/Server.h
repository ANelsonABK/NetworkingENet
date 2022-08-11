#pragma once
#include "Packets.h"
#include "enet/enet.h"
#include <iostream>
#include <atomic>

class Server {

	int _numMaxConnections;
	int _numCurrConections;
	ENetHost* _server;
	int _randomNumber;
	std::atomic<bool> _isServer;

public:
	Server(int numConnections);
	~Server();

	// Functions
	bool CreateServer();
	void ServerProcessPackets();
	void DisconnectClient(ENetEvent event);
	void ConnectClient(ENetEvent event);

	void BroadcastIsCorrectPacket();
	void BroadcastPlayerPacket();
	void HandleReceivePacket(const ENetEvent& event);

	void SetRandomNumber() { _randomNumber = (rand() % 100); }
	int GetRandomNumber() { return _randomNumber; }
	ENetHost* GetServer() { return _server; }
	void SetServerStatus() {
		bool serverStatus = _isServer.load();
		_isServer.store(!serverStatus);
	}
	//int GetMaxNumConnections() { return _numMaxConnections; }
	//int GetCurrNumConnections() { return _numCurrConections; }
};