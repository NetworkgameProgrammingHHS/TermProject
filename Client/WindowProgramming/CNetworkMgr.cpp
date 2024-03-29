#include "pch.h"
#include "CNetworkMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CGun.h"

CNetworkMgr::CNetworkMgr()
{
	InitializeCriticalSection(&g_CS);

	m_sfFont.loadFromFile("Resource\\Font\\cour.ttf");
	m_sfPlayerColor[0].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(0, 0, 32, 32)); // Normal
	m_sfPlayerColor[1].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(32, 0, 32, 32)); // Red
	m_sfPlayerColor[2].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(0, 32, 32, 32)); // Green
	m_sfPlayerColor[3].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(32, 32, 32, 32)); // Blue
	m_sfPlayerColor[4].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(0, 64, 32, 32)); // GB
	m_sfPlayerColor[5].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(32, 64, 32, 32)); // Yellow
	m_sfPlayerColor[6].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(0, 96, 32, 32)); // Cyan

	m_sfPlayerInfoText[0][1].setString("Stage1");
	m_sfPlayerInfoText[1][1].setString("Stage1");
	m_sfPlayerInfoText[2][1].setString("Stage1");

	for (int i = 0; i < 3; ++i) {
		m_sfPlayerInfo[i].setTexture(m_sfPlayerColor[0]);
		m_sfPlayerInfo[i].setScale(2, 2);
		m_sfPlayerInfo[i].setPosition(WINDOW_WIDTH - (m_sfPlayerInfo[0].getScale().x * TILE_SIZE * (3 - i)), 0);

		for (int j = 0; j < 2; ++j) {
			m_sfPlayerInfoText[i][j].setOutlineThickness(1.0f);
			m_sfPlayerInfoText[i][j].setCharacterSize(12);
			m_sfPlayerInfoText[i][j].setFont(m_sfFont);
			m_sfPlayerInfoText[i][j].setFillColor(sf::Color::White);
			auto size = m_sfPlayerInfoText[i][j].getGlobalBounds();
			m_sfPlayerInfoText[i][j].setPosition(WINDOW_WIDTH - (m_sfPlayerInfo[0].getScale().x * TILE_SIZE * (3 - i)) + 5, TILE_SIZE * 2 + j * 14 + 5 * (j + 1));
		}
	}

	m_pGun = make_shared<CGun>();
}

CNetworkMgr::~CNetworkMgr()
{
	WSACleanup();
	DeleteCriticalSection(&g_CS);
}

void CNetworkMgr::InitializeSocket(string ip)
{
	
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
		cout << "WSAStartup fail" << endl;

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
		cout << "socket error" << endl;

	/*int opt = TRUE;
	setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt, sizeof(opt));*/

	memset(&m_serveraddr, 0, sizeof(m_serveraddr));
	m_serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &m_serveraddr.sin_addr);
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
		SC_LOGIN_INFO_PACKET* packet = reinterpret_cast<SC_LOGIN_INFO_PACKET*>(buf);
		// 로그인 이후 게임 대기 방에서 플레이어가 들어온 순서대로 캐릭터를 렌더한다.
		if (packet->online_p1 == CLIENT_ONLINE)
		{
			// 플레이어 1 클라이언트 화면에 로그인 하였다고 표시, 렌더
			players[0]->SetOnline(true);
			SetPlayerName(packet->name_p1, 0);
			players[0]->SetReady(packet->ready_p1);
		}
		if (packet->online_p2 == CLIENT_ONLINE)
		{
			// 플레이어 2 클라이언트 화면에 로그인 하였다고 표시, 렌더
			players[1]->SetOnline(true);
			SetPlayerName(packet->name_p2, 1);
			players[1]->SetReady(packet->ready_p2);
		}
		if (packet->online_p3 == CLIENT_ONLINE)
		{
			// 플레이어 3 클라이언트 화면에 로그인 하였다고 표시, 렌더
			players[2]->SetOnline(true);
			players[2]->SetReady(packet->ready_p3);
			SetPlayerName(packet->name_p3, 2);
		}

		EnterCriticalSection(&g_CS);
		if (m_nPlayerIndex == -1 && packet->id != -1) {
			m_nPlayerIndex = packet->id;
			scene->SetNext(true);
		}
		LeaveCriticalSection(&g_CS);
		break;
	}
	case SC_READY: {
		SC_READY_PACKET* packet = reinterpret_cast<SC_READY_PACKET*>(buf);
		// 로그인 인포 패킷을 받아 로그인 한 플레이어의 순서에 따라 준비 상태를 출력한다.
		if (packet->ready == READY_OFF) 
		{ 
			players[packet->id]->SetReady(false);
		}
		if (packet->ready == READY_ON)
		{ 
			players[packet->id]->SetReady(true);			
		}
		break;
	}
	case SC_GAMESTART: {		
		Sleep(400);
		SC_GAMESTART_PACKET* packet = reinterpret_cast<SC_GAMESTART_PACKET*>(buf);
		scene->SetNext(true);
		Sleep(500);
		break;
	}
	case SC_WORLD_UPDATE: {
		SC_WORLD_UPDATE_PACKET* packet = reinterpret_cast<SC_WORLD_UPDATE_PACKET*>(buf);

		if (players[0]->GetStageNum() != static_cast<SCENE_NUM>(packet->stage_p1 + 2)) {
			players[0]->SetStage(static_cast<SCENE_NUM>(packet->stage_p1 + 2));
			SetPlayerInfo(static_cast<SCENE_NUM>(packet->stage_p1 + 2), 0);
		}
		if (players[1]->GetStageNum() != static_cast<SCENE_NUM>(packet->stage_p2 + 2)) {
			players[1]->SetStage(static_cast<SCENE_NUM>(packet->stage_p2 + 2));
			SetPlayerInfo(static_cast<SCENE_NUM>(packet->stage_p2 + 2), 1);
		}
		if (players[2]->GetStageNum() != static_cast<SCENE_NUM>(packet->stage_p3 + 2)) {
			players[2]->SetStage(static_cast<SCENE_NUM>(packet->stage_p3 + 2));
			SetPlayerInfo(static_cast<SCENE_NUM>(packet->stage_p3 + 2), 2);
		}

		if (scene->GetSceneNum() == players[0]->GetStageNum())
			players[0]->SetPosition(sf::Vector2f((float)packet->x_p1, (float)packet->y_p1));
		if (scene->GetSceneNum() == players[1]->GetStageNum())
			players[1]->SetPosition(sf::Vector2f((float)packet->x_p2, (float)packet->y_p2));
		if (scene->GetSceneNum() == players[2]->GetStageNum())
			players[2]->SetPosition(sf::Vector2f((float)packet->x_p3, (float)packet->y_p3));

		if ((packet->dir_p1 == LEFT || packet->dir_p1 == RIGHT) && scene->GetSceneNum() == players[0]->GetStageNum()) {
			players[0]->SetDir((int)packet->dir_p1);
		}
		
		if ((packet->dir_p2 == LEFT || packet->dir_p2 == RIGHT) && scene->GetSceneNum() == players[1]->GetStageNum()) {
			players[1]->SetDir((int)packet->dir_p2);
		}
		
		if ((packet->dir_p3 == LEFT || packet->dir_p3 == RIGHT) && scene->GetSceneNum() == players[2]->GetStageNum()) {
			players[2]->SetDir((int)packet->dir_p3);
		}


		SCENE_NUM packetSceneNum = SCENE_NUM::NONE;
		switch (packet->stage_bullet)
		{
		case STAGE_1:
			packetSceneNum = SCENE_NUM::STAGE1;
			break;
		case STAGE_2:
			packetSceneNum = SCENE_NUM::STAGE2;
			break;
		case STAGE_3:
			packetSceneNum = SCENE_NUM::STAGE3;
			break;
		case STAGE_4:
			packetSceneNum = SCENE_NUM::STAGE4;
			break;
		case STAGE_5:
			packetSceneNum = SCENE_NUM::STAGE5;
			break;
		case STAGE_END:
			packetSceneNum = SCENE_NUM::RANKING;
			break;
		default:
			break;
		}
		SetGunState((int)packet->bullet_enable, (int)packet->x_bullet, (int)packet->y_bullet);
		m_pGun->SetGunStage(packetSceneNum);
		break;
	}
	case SC_RANK: {
		SC_RANK_PACKET* packet = reinterpret_cast<SC_RANK_PACKET*>(buf);
		m_WinnerPlayer.setString(packet->winner_name);
		break;
	}
	case SC_REMOVE: {
		SC_REMOVE_PACKET* packet = reinterpret_cast<SC_REMOVE_PACKET*>(buf);
		scene->Logout(packet->id);
		break;
	}
	case SC_COLOR:
	{
		SC_COLOR_PACKET* packet = reinterpret_cast<SC_COLOR_PACKET*>(buf);
		players[packet->id]->SetColor(static_cast<PLAYER_COLOR>(packet->color));
		SetPlayerInfo(static_cast<PLAYER_COLOR>(packet->color), packet->id);
		break;
	}
	case SC_RESET: {
		SC_RESET_PACKET* packet = reinterpret_cast<SC_RESET_PACKET*>(buf);
		players[packet->id]->Reset();
		if(packet->id == m_nPlayerIndex)
			scene->TileReset();
		SetPlayerInfo(PLAYER_COLOR::NORMAL, packet->id);
		break;
	}
	}
}

void CNetworkMgr::SetPlayerInfo(const PLAYER_COLOR pc, const int index)
{
	EnterCriticalSection(&g_CS);
	switch (pc) {
	case PLAYER_COLOR::NORMAL:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[0]);
		break;
	case PLAYER_COLOR::RED:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[1]);
		break;
	case PLAYER_COLOR::GREEN:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[2]);
		break;
	case PLAYER_COLOR::BLUE:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[3]);
		break;
	case PLAYER_COLOR::GB:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[4]);
		break;
	case PLAYER_COLOR::YELLOW:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[5]);
		break;
	case PLAYER_COLOR::PURPLE:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[6]);
		break;
	default:
		break;
	}
	LeaveCriticalSection(&g_CS);
}

void CNetworkMgr::SetPlayerInfo(const SCENE_NUM sn, const int index)
{
	EnterCriticalSection(&g_CS);
	switch (sn) {
	case SCENE_NUM::STAGE1:
		m_sfPlayerInfoText[index][1].setString("Stage 1");
		break;
	case SCENE_NUM::STAGE2:
		m_sfPlayerInfoText[index][1].setString("Stage 2");
		break;
	case SCENE_NUM::STAGE3:
		m_sfPlayerInfoText[index][1].setString("Stage 3");
		break;
	case SCENE_NUM::STAGE4:
		m_sfPlayerInfoText[index][1].setString("Stage 4");
		break;
	case SCENE_NUM::STAGE5:
		m_sfPlayerInfoText[index][1].setString("Stage 5");
		break;
	default:
		break;
	}
	LeaveCriticalSection(&g_CS);
}

void CNetworkMgr::SetPlayerName(const char* name, const int index)
{
	EnterCriticalSection(&g_CS);
	m_sfPlayerInfoText[index][0].setString(name);
	LeaveCriticalSection(&g_CS);
}

void CNetworkMgr::SetWinnerName(const sf::String name)
{
	EnterCriticalSection(&g_CS);
	m_WinnerPlayer.setString(name);
	LeaveCriticalSection(&g_CS);
}

void CNetworkMgr::SetGunState(int enable, int bulletx, int bullety)
{
	if (m_pGun)
	{
		m_pGun->SetEnable(enable);
		m_pGun->SetBulletPos(bulletx, bullety);
	}
}