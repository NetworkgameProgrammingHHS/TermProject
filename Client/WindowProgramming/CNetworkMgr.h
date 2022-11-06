#pragma once

class CScene;
class CPlayer;

class CNetworkMgr
{
public:
	CNetworkMgr();
	~CNetworkMgr();

	void InitializeSocket();
	void SendPacket(char* packet, int size);
	void RecvPacket(CScene* scene, CPlayer* player);

	const SOCKET GetSocket() const { return m_sock; };

private:
	WSADATA m_wsa;
	SOCKET m_sock;
	SOCKADDR_IN m_serveraddr;
};

