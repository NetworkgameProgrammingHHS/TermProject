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

	const int GetPlayerIndex() const { return m_nPlayerIndex; }
	void SetPlayerIndex(int index) { m_nPlayerIndex = index; }

private:
	WSADATA m_wsa;
	SOCKET m_sock;
	SOCKADDR_IN m_serveraddr;

	CRITICAL_SECTION g_CS;

	int m_nPlayerIndex = -1;
};

