#include "pch.h"
#include "TileMap.h"
#include "Bullet.h"
#include "Player.h"

//Global Variable
int g_iCntClientNum = 0;
array<Player, PLAYER_NUM> g_Clients;

array<unique_ptr<TileMap>, STAGE_NUM> g_TileMap;
unique_ptr<Bullet> g_Bullet;

float g_ElapsedTime;

bool g_GameOver;
bool g_InGame = false;
CRITICAL_SECTION g_CS;

//Client Update Thread
DWORD WINAPI ProcessPacket(LPVOID socket);
//if complete, delete
void err_display(const char* msg);
void err_display(int errcode);
chrono::time_point<chrono::system_clock> startTime;

// Packet Send Thread
DWORD WINAPI SendPacket(LPVOID)
{
	SC_WORLD_UPDATE_PACKET* packet = new SC_WORLD_UPDATE_PACKET;
	int update_len = sizeof(SC_WORLD_UPDATE_PACKET);
	packet->type = SC_WORLD_UPDATE;
	while (true) {
		// send packet per 1/30sec --> 33.3 ms

		auto endTime = chrono::system_clock::now();
		g_ElapsedTime += chrono::duration<float, milli>(endTime - startTime).count();
		startTime = endTime;

		if ((g_ElapsedTime - 33.3f) >= DBL_EPSILON) {
			if (g_iCntClientNum > 0) {
				if (g_Clients[0].GetOnline()) {
					packet->color_p1 = static_cast<short>(g_Clients[0].GetColor	());
					packet->dir_p1 = g_Clients[0].GetDirection();
					packet->stage_p1 = g_Clients[0].GetStageNum();
					packet->x_p1 = g_Clients[0].GetPos().x;
					packet->y_p1 = g_Clients[0].GetPos().y;
				}
				if (g_Clients[1].GetOnline()) {
					packet->color_p2 = static_cast<short>(g_Clients[1].GetColor());
					packet->dir_p2 = g_Clients[1].GetDirection();
					packet->stage_p2 = g_Clients[1].GetStageNum();
					packet->x_p2 = g_Clients[1].GetPos().x;
					packet->y_p2 = g_Clients[1].GetPos().y;
				}
				if (g_Clients[2].GetOnline()) {
					packet->color_p3 = static_cast<short>(g_Clients[2].GetColor());
					packet->dir_p3 = g_Clients[2].GetDirection();
					packet->stage_p3 = g_Clients[2].GetStageNum();
					packet->x_p2 = g_Clients[2].GetPos().x;
					packet->y_p2 = g_Clients[2].GetPos().y;
				}
				if (g_Bullet) {
					packet->bullet_enable = BULLET_ON;
					packet->dir_bullet = g_Bullet->GetDirection();
					packet->stage_bullet = g_Bullet->GetStage();
					packet->x_bullet = g_Bullet->GetPos().x;
					packet->y_bullet = g_Bullet->GetPos().y;
				}
				else {
					packet->bullet_enable = BULLET_OFF;
				}
				for (int i = 0; i < PLAYER_NUM; ++i) {
					if (g_Clients[i].GetOnline()) {
						send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(&update_len), sizeof(int), 0);
						send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(packet), update_len, 0);
					}
				}
			}

			g_ElapsedTime = 0.f;
		}
	}


	delete packet;
	return 0;
}

int main()
{
	float ElapsedTime = 0.f;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		cout << "socket error" << endl;

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	int ret = bind(listen_sock, reinterpret_cast<struct sockaddr*>(&serveraddr), sizeof(serveraddr));
	if (ret == SOCKET_ERROR)
		cout << "bind error" << endl;

	ret = listen(listen_sock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
		cout << "listen error" << endl;

	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	startTime = chrono::system_clock::now();

	int index = 0;
	while (!g_InGame) {
		if (g_iCntClientNum > 3)
			continue;
		else {
			for (int i = 0; i < PLAYER_NUM; ++i) {
				if (!g_Clients[i].GetOnline()) {
					index = i;
					break;
				}
			}
		}

		addrlen = sizeof(clientaddr);
		g_Clients[index].SetSocket(accept(listen_sock, reinterpret_cast<struct sockaddr*>(&clientaddr), &addrlen));
		if (g_Clients[index].GetSocket() == INVALID_SOCKET) {
			cout << "accept error" << endl;
			exit(1);
		}
		//set client id
		g_Clients[index].SetID(index);
		g_iCntClientNum++;
		
		DWORD ThreadId;
		hThread = CreateThread(NULL, 0, ProcessPacket, reinterpret_cast<LPVOID>(g_Clients[index].GetSockInfo()), 0, &ThreadId);

		if (NULL == hThread) { closesocket(g_Clients[index].GetSocket()); }
		else { CloseHandle(hThread); }

		hThread = CreateThread(NULL, 0, SendPacket, NULL, 0, 0);
		if (NULL == hThread) { cout << "failed to create Thread" << endl; }
		else { CloseHandle(hThread); }
	}
	
	

	auto endTime = chrono::system_clock::now();
	auto StartT = endTime;
	while (true) {
		ElapsedTime = chrono::duration<float, milli>(endTime - StartT).count();
		//InGame
		// Bullet, Move, Collide
		if (g_Bullet) {
			int dir = 0;
			if (g_Bullet->GetDirection() == KEY_DIR_LEFT)
				dir = -1;
			else if (g_Bullet->GetDirection() == KEY_DIR_RIGHT)
				dir = 1;
			
			g_Bullet->SetPos({ g_Bullet->GetPos().x + dir * ElapsedTime, g_Bullet->GetPos().y});
		}

		// Player, Move, Collides

		for (int i = 0; i < PLAYER_NUM; ++i) {
			g_Clients[i].SetPos({g_Clients[i].GetPos().x + g_Clients[i].GetDirection() * ElapsedTime, g_Clients[i].GetPos().y});
		}

		StartT = endTime;
		endTime = chrono::system_clock::now();
	}

	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

DWORD WINAPI ProcessPacket(LPVOID socket)
{
	int retval;
	SOCK_INFO* sock_info = reinterpret_cast<SOCK_INFO*> (socket);

	SOCKET client_sock = sock_info->client_sock;
	int len;
	char buf[BUF_SIZE];
	PLAYER_COLOR* packet_color = new PLAYER_COLOR;
	cout << "id: " << sock_info->id << endl;
	//When the players first come in, set stage 1
	g_Clients[sock_info->id].SetStageNum(STAGE_1);
	while (1)
	{
		//recv packet size
		retval = recv(client_sock, (char*)(&len), sizeof(int), MSG_WAITALL);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;
		//recv packet
		retval = recv(client_sock, buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		switch (buf[0])
		{
		case CS_LOGIN://
		{
			CS_LOGIN_PACKET* packet = reinterpret_cast<CS_LOGIN_PACKET*>(buf);
			g_Clients[sock_info->id].SetName(packet->name);
			g_Clients[sock_info->id].SetOnline(true);

			// Send Login Info packet to Client which logined
			SC_LOGIN_INFO_PACKET* scp = new SC_LOGIN_INFO_PACKET;
			scp->type = SC_LOGIN_INFO;
			scp->id = sock_info->id;
			scp->online_p1 = g_Clients[0].GetOnline();
			scp->online_p2 = g_Clients[1].GetOnline();
			scp->online_p3 = g_Clients[2].GetOnline();
			len = sizeof(SC_LOGIN_INFO_PACKET);
			send(sock_info->client_sock, reinterpret_cast<char*>(&len), sizeof(len), 0);
			send(sock_info->client_sock, reinterpret_cast<char*>(scp), len, 0);
			delete scp;

			break;
		}
		case CS_PLAYER_READY:
		{
			CS_PLAYER_READY_PACKET* packet = reinterpret_cast<CS_PLAYER_READY_PACKET*>(buf);
			if (packet->ready == READY_ON) {
				g_Clients[sock_info->id].SetReady(true);

				// Send Ready packet to Clients
				SC_READY_PACKET* scp = new SC_READY_PACKET;
				scp->type = READY_ON;
				scp->id = sock_info->id;
				len = sizeof(SC_READY_PACKET);
				for (int i = 0; i < 3; ++i) {
					send(g_Clients[i].GetSockInfo()->client_sock, reinterpret_cast<char*>(&len), sizeof(len), 0);
					send(g_Clients[i].GetSockInfo()->client_sock, reinterpret_cast<char*>(scp), len, 0);
				}
			}
			else if (packet->ready == READY_OFF) {
				g_Clients[sock_info->id].SetReady(false);

				// Send Ready packet to Clients
				SC_READY_PACKET* scp = new SC_READY_PACKET;
				scp->type = READY_OFF;
				scp->id = sock_info->id;
				len = sizeof(SC_READY_PACKET);
				for (int i = 0; i < 3; ++i) {
					send(g_Clients[i].GetSockInfo()->client_sock, reinterpret_cast<char*>(&len), sizeof(len), 0);
					send(g_Clients[i].GetSockInfo()->client_sock, reinterpret_cast<char*>(scp), len, 0);
				}
			}

			break;
		}
		case CS_INPUT:
		{
			CS_INPUT_PACKET* packet = reinterpret_cast<CS_INPUT_PACKET*>(buf);
			//input key
			if (packet->state == KEY_PRESS)
			{
				g_Clients[sock_info->id].SetKeyState(true);
				if (packet->type == KEY_FIREGUN)
				{
					if (g_Clients[sock_info->id].GetGun())
					{
						//creat gun and shooting bullet
					}
				}
				else {
					switch (packet->key) {
					case KEY_DIR_LEFT:
						g_Clients[sock_info->id].SetDirection(LEFT);
						break;
					case KEY_DIR_RIGHT:
						g_Clients[sock_info->id].SetDirection(RIGHT);
						break;
					default:
						break;
					}
				}
			}
			else if (packet->state == KEY_RELEASE)
			{
				g_Clients[sock_info->id].SetKeyState(false);
				if (packet->type == KEY_FIREGUN);
				else
					g_Clients[sock_info->id].SetDirection(0);
			}
			break;
		}
		case CS_COLOR:
		{
			CS_PLAYER_COLOR_PACKET* packet = reinterpret_cast<CS_PLAYER_COLOR_PACKET*>(buf);
			packet_color = reinterpret_cast<PLAYER_COLOR*>(packet->color);
			//input color
			g_Clients[sock_info->id].SetColor(*packet_color);
			break;
		}
		case CS_PLAYER_RESET:
		{
			CS_PLAYER_RESET_PACKET* packet = reinterpret_cast<CS_PLAYER_RESET_PACKET*>(buf);
			if (packet->reset == RESET_ON)
			{
				//player go back to original position
				//g_Clients[sock_info->id].SetPos(0);
				//set original color
				g_Clients[sock_info->id].SetColor(PLAYER_COLOR::NORMAL);
				g_Clients[sock_info->id].SetDirection(0);
			}
			break;
		}
		case CS_NEXTSTAGE:
		{
			switch (g_Clients[sock_info->id].GetStageNum())
			{
			case STAGE_1:
				g_Clients[sock_info->id].SetStageNum(STAGE_2);
				break;
			case STAGE_2:
				g_Clients[sock_info->id].SetStageNum(STAGE_3);
				break;
			case STAGE_3:
				g_Clients[sock_info->id].SetStageNum(STAGE_4);
				break;
			case STAGE_4:
				g_Clients[sock_info->id].SetStageNum(STAGE_5);
				break;
			case STAGE_5:
				g_Clients[sock_info->id].SetStageNum(STAGE_END);
				//send rank all player
				break;
			default:
				break;
			}
			break;
		}
		}
	}
	closesocket(client_sock);
	return 0;
}


// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
