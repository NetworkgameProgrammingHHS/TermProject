#pragma once

class CScene;
class CPlayer;
class CGun;

class CNetworkMgr
{
public:
	CNetworkMgr();
	virtual ~CNetworkMgr();

	void InitializeSocket(string ip);
	void SendPacket(char* packet, int size);
	void RecvPacket(CScene* scene, array<shared_ptr<CPlayer>, PLAYERNUM> players);
	
	SOCKET GetSocket() { return m_sock; };

	const int GetPlayerIndex() const { return m_nPlayerIndex; }
	const shared_ptr<CGun> GetGun() const { return m_pGun; }
	void SetPlayerIndex(int index) { m_nPlayerIndex = index; }

	void SetPlayerInfo(const PLAYER_COLOR pc, const int index);
	void SetPlayerInfo(const SCENE_NUM sn, const int index);
	void SetPlayerName(const char* name, const int index);
	void SetWinnerName(const sf::String name);
	void SetGunState(int enable, int bulletx, int bullety);

	const array<sf::Sprite, 3> GetPlayerInfo() const { return m_sfPlayerInfo; }
	const array<array<sf::Text, 2>, 3> GetPlayerInfoText() const {return m_sfPlayerInfoText; }
	const sf::Text GetWinnerName()const { return m_WinnerPlayer; }

private:
	WSADATA m_wsa = {};
	SOCKET m_sock = {};
	SOCKADDR_IN m_serveraddr = {};

	CRITICAL_SECTION g_CS = {};

	sf::Font m_sfFont = {};
	array<sf::Texture, 7> m_sfPlayerColor = {};
	array<sf::Sprite, 3> m_sfPlayerInfo = {};
	array<array<sf::Text, 2>, 3> m_sfPlayerInfoText = {};

	int m_nPlayerIndex = -1;
	sf::Text m_WinnerPlayer = {};
	shared_ptr<CGun> m_pGun = nullptr;
};

