#pragma once
#include "Packets.h"
#include "enet/enet.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <string>

class Client {

	int _playerId;
	u_int _clientId;
	std::atomic<bool> _quitGame; /* keeps player client up */
	std::atomic<bool> _canAnswer; /* lets player know they can answer */
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
	void SendPacket(string guess);
	ENetHost* GetClient() { return _client; }
};