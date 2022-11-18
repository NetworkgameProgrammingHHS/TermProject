#pragma once

class CScene;
class CPlayer;

class CNetworkMgr
{
public:
	CNetworkMgr();
	virtual ~CNetworkMgr();

	void InitializeSocket();
	void SendPacket(char* packet, int size);
	void RecvPacket(CScene* scene, array<shared_ptr<CPlayer>, PLAYERNUM> players);
	
	SOCKET GetSocket() { return m_sock; };

private:
	WSADATA m_wsa;
	SOCKET m_sock;
	SOCKADDR_IN m_serveraddr;
};

