#pragma once
#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <chrono>

class Server
{
	bool m_isServerRunning;
public:

	Server(bool isClientRunning);
	~Server();

	bool CreateServer(ENetAddress* address);
	void ConnectClient(ENetEvent event, ENetAddress address);
	void RecieveMessage(ENetEvent event);
	void DisconnectClient(ENetEvent event);

	bool GetIsClientRunning() { return m_isServerRunning; }
	void SetIsClientRunning() { m_isServerRunning = !m_isServerRunning; }
	ENetHost* GetServer() { return m_server; }
	void RunServerThread();

protected:
	ENetHost* m_server;
};
