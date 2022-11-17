#include "pch.h"
#include "CNetworkMgr.h"
#include "CScene.h"
#include "CPlayer.h"

CNetworkMgr::CNetworkMgr()
{
}

CNetworkMgr::~CNetworkMgr()
{
}

void CNetworkMgr::InitializeSocket()
{
	
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
		cout << "WSAStartup fail" << endl;

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
		cout << "socket error" << endl;
	
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

void CNetworkMgr::RecvPacket(CScene* scene, CPlayer* player)
{
	int len;
	char buf[BUF_SIZE];

	recv(m_sock, (char*)(&len), sizeof(int), MSG_WAITALL);
	recv(m_sock, buf, len, MSG_WAITALL);

	switch (buf[0]) {
	case SC_LOGIN_INFO: {
		SC_LOGIN_INFO_PACKET* packet = reinterpret_cast<SC_LOGIN_INFO_PACKET*>(buf);
		// �α��� ���� ���� ��� �濡�� �÷��̾ ���� ������� ĳ���͸� �����Ѵ�.
		if (packet->online_p1 == CLIENT_ONLINE)
		{
			// �÷��̾� 1 Ŭ���̾�Ʈ ȭ�鿡 �α��� �Ͽ��ٰ� ǥ��, ����
		}
		if (packet->online_p2 == CLIENT_ONLINE)
		{
			// �÷��̾� 2 Ŭ���̾�Ʈ ȭ�鿡 �α��� �Ͽ��ٰ� ǥ��, ����
		}
		if (packet->online_p3 == CLIENT_ONLINE)
		{
			// �÷��̾� 3 Ŭ���̾�Ʈ ȭ�鿡 �α��� �Ͽ��ٰ� ǥ��, ����
		}
		std::cout << packet->id << "����" << std::endl;
		scene->SetPlayerIndex(packet->id);
		break;
	}
	case SC_READY: {
		SC_READY_PACKET* packet = reinterpret_cast<SC_READY_PACKET*>(buf);
		// �α��� ���� ��Ŷ�� �޾� �α��� �� �÷��̾��� ������ ���� �غ� ���¸� ����Ѵ�.
		if (packet->type == READY_OFF) { std::cout << "�غ� ����" << std::endl; }
		if (packet->type == READY_ON) { std::cout << "�غ�" << std::endl; }
		break;
	}
	case SC_GAMESTART: {
		SC_GAMESTART_PACKET* packet = reinterpret_cast<SC_GAMESTART_PACKET*>(buf);
		scene->SetNext(true);
		break;
	}
	case SC_WORLD_UPDATE: {
		SC_WORLD_UPDATE_PACKET* packet = reinterpret_cast<SC_WORLD_UPDATE_PACKET*>(buf);
		int curPlayerIndex = scene->GetPlayerIndex();
		switch (curPlayerIndex)
		{
		case 0:
		{
			player->SetColor((PLAYER_COLOR)packet->color_p1);
			player->SetPosition(sf::Vector2f((float)packet->x_p1, (float)packet->y_p1));
			player->SetDir((int)packet->dir_p1);
		}
			break;

		case 1:
		{
			player->SetColor((PLAYER_COLOR)packet->color_p2);
			player->SetPosition(sf::Vector2f((float)packet->x_p2, (float)packet->y_p2));
			player->SetDir((int)packet->dir_p2);
		}
			break;

		case 2:
		{
			player->SetColor((PLAYER_COLOR)packet->color_p2);
			player->SetPosition(sf::Vector2f((float)packet->x_p2, (float)packet->y_p2));
			player->SetDir((int)packet->dir_p2);
		}
			break;
		}
		
		scene->SetGunState((int)packet->bullet_enable, (int)packet->x_bullet, (int)packet->y_bullet);

		break;
	}
	case SC_RANK: {
		SC_RANK_PACKET* packet = reinterpret_cast<SC_RANK_PACKET*>(buf);
		// Show Ranking Scene
		break;
	}
	case SC_REMOVE: {
		SC_REMOVE_PACKET* packet = reinterpret_cast<SC_REMOVE_PACKET*>(buf);
		std::cout << packet->id << "�α׾ƿ�" << std::endl;
		scene->Logout((int)packet->id);
		
		break;
	}
	}
}
