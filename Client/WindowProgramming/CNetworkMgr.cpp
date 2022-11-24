#include "pch.h"
#include "CNetworkMgr.h"
#include "CScene.h"
#include "CPlayer.h"

CNetworkMgr::CNetworkMgr()
{
	InitializeCriticalSection(&g_CS);
}

CNetworkMgr::~CNetworkMgr()
{
	DeleteCriticalSection(&g_CS);
}

void CNetworkMgr::InitializeSocket()
{
	
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
		cout << "WSAStartup fail" << endl;

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
		cout << "socket error" << endl;

	//int op = 1;
	//setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&op), sizeof(op));

	memset(&m_serveraddr, 0, sizeof(m_serveraddr));
	m_serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &m_serveraddr.sin_addr);
	m_serveraddr.sin_port = htons(SERVER_PORT);
	int ret = connect(m_sock, (sockaddr*)&m_serveraddr, sizeof(m_serveraddr));
	if (ret == SOCKET_ERROR)
		cout << "connect error" << endl;
		
}

void CNetworkMgr::SendPacket(char* packet, int size)
{
	send(m_sock, reinterpret_cast<char*>(&size), sizeof(int), 0);
	send(m_sock, packet, size, 0);
	delete packet;
}

void CNetworkMgr::RecvPacket(CScene* scene, array<shared_ptr<CPlayer>, PLAYERNUM> players)
{
	if (!scene)
		return;

	int len;
	char buf[BUF_SIZE];

	recv(m_sock, reinterpret_cast<char*>(&len), sizeof(int), MSG_WAITALL);
	recv(m_sock, buf, len, MSG_WAITALL);

	switch (buf[0]) {
	case SC_LOGIN_INFO: {
		cout << "SC_LOGIN_INFO" << endl;
		SC_LOGIN_INFO_PACKET* packet = reinterpret_cast<SC_LOGIN_INFO_PACKET*>(buf);
		// 로그인 이후 게임 대기 방에서 플레이어가 들어온 순서대로 캐릭터를 렌더한다.
		if (packet->online_p1 == CLIENT_ONLINE)
		{
			// 플레이어 1 클라이언트 화면에 로그인 하였다고 표시, 렌더
			players[0]->SetOnline(true);
		}
		if (packet->online_p2 == CLIENT_ONLINE)
		{
			// 플레이어 2 클라이언트 화면에 로그인 하였다고 표시, 렌더
			players[1]->SetOnline(true);
		}
		if (packet->online_p3 == CLIENT_ONLINE)
		{
			// 플레이어 3 클라이언트 화면에 로그인 하였다고 표시, 렌더
			players[2]->SetOnline(true);
		}
		std::cout << packet->id << "접속" << std::endl;

		EnterCriticalSection(&g_CS);
		if(m_nPlayerIndex == -1)
			m_nPlayerIndex = packet->id;
		LeaveCriticalSection(&g_CS);
		break;
	}
	case SC_READY: {
		cout << "SC_READY" << endl;
		SC_READY_PACKET* packet = reinterpret_cast<SC_READY_PACKET*>(buf);
		// 로그인 인포 패킷을 받아 로그인 한 플레이어의 순서에 따라 준비 상태를 출력한다.
		if (packet->ready == READY_OFF) { std::cout << "준비 안함" << std::endl; }
		if (packet->ready == READY_ON) { std::cout << "준비" << std::endl; }
		break;
	}
	case SC_GAMESTART: {
		cout << "SC_GAMESTART" << endl;
		SC_GAMESTART_PACKET* packet = reinterpret_cast<SC_GAMESTART_PACKET*>(buf);
		scene->SetNext(true);
		break;
	}
	case SC_WORLD_UPDATE: {
		//EnterCriticalSection(&g_CS);
		SC_WORLD_UPDATE_PACKET* packet = reinterpret_cast<SC_WORLD_UPDATE_PACKET*>(buf);
		//int curPlayerIndex = m_nPlayerIndex;

		players[0]->SetPosition(sf::Vector2f((float)packet->x_p1, (float)packet->y_p1));
		if (packet->dir_p1 == LEFT || packet->dir_p1 == RIGHT) {
			players[0]->SetDir((int)packet->dir_p1);
		}
		
		players[1]->SetPosition(sf::Vector2f((float)packet->x_p2, (float)packet->y_p2));
		if (packet->dir_p2 == LEFT || packet->dir_p2 == RIGHT) {
			players[1]->SetDir((int)packet->dir_p2);
		}


		players[2]->SetPosition(sf::Vector2f((float)packet->x_p3, (float)packet->y_p3));
		if (packet->dir_p3 == LEFT || packet->dir_p3 == RIGHT) {
			players[2]->SetDir((int)packet->dir_p3);
		}

		scene->SetGunState((int)packet->bullet_enable, (int)packet->x_bullet, (int)packet->y_bullet);
		//LeaveCriticalSection(&g_CS);
		break;
	}
	case SC_RANK: {
		cout << "SC_RANK" << endl;
		SC_RANK_PACKET* packet = reinterpret_cast<SC_RANK_PACKET*>(buf);
		// Show Ranking Scene
		break;
	}
	case SC_REMOVE: {
		cout << "SC_REMOVE" << endl;
		SC_REMOVE_PACKET* packet = reinterpret_cast<SC_REMOVE_PACKET*>(buf);
		std::cout << packet->id << "로그아웃" << std::endl;
		//scene->Logout((int)packet->id);
		
		break;
	}
	case SC_COLOR:
	{
		SC_COLOR_PACKET* packet = reinterpret_cast<SC_COLOR_PACKET*>(buf);
		players[packet->id]->SetColor(static_cast<PLAYER_COLOR>(packet->color));
		break;
	}
	}
}
