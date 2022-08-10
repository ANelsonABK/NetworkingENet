#pragma once
#include <iostream>
#include <thread>
#include <enet/enet.h>
#include <string>
//#include <mutex>
//#include <atomic>

class Client
{
	int m_id;
	std::string m_username;
	
public:
	Client(int id, std::string username);
	~Client();

	bool CreateClient();
	void SetUserName(std::string name) { m_username = name; }
	int GetClientID() { return m_id; }
	//bool GetIsRunning() { return m_isRunning; }
	//void SetIsRunning() { m_isRunning = !m_isRunning; }
	ENetHost* GetClient() { return m_client; }

	std::string GetUserName() { return m_username; }
	void SendPacket(ENetPeer* peer, const char* data, int channelID = 0);
	void MessageLoop();
	void InputLoop(ENetPeer* peer);
	void ConnectToServer();

protected:
	ENetHost* m_client;
};

