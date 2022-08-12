#pragma once
#include "Packets.h"
#include "enet/enet.h"
#include <iostream>
#include <thread>

class Client {

	int _playerId;
	u_int _clientId;
	ENetHost* _client;
	ENetPeer* _peer;

public:
	Client();
	~Client();

	bool CreateClient();
	bool ConnectToServer();

	void ClientProcessPackets();
	void HandleReceivePacket(ENetEvent& event);
	void InputLoop();
	void SendPacket(std::string guess);
	ENetHost* GetClient() { return _client; }
};