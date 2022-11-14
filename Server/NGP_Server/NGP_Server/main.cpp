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
	while (true) {
		// send packet per 1/30sec --> 33.3 ms

		auto endTime = chrono::system_clock::now();
		g_ElapsedTime += chrono::duration<float, milli>(endTime - startTime).count();
		startTime = endTime;

		if ((g_ElapsedTime - 33.3f) >= DBL_EPSILON) {
			if (g_iCntClientNum > 0) {
				for (int i = 0; i < PLAYER_NUM; ++i) {
					if (g_Clients[i].GetOnline()) {

					}
				}
			}

			g_ElapsedTime = 0.f;
		}
	}


	return 0;
}

int main()
{
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
	hThread = CreateThread(NULL, 0, SendPacket, NULL, 0, 0);
	if (NULL == hThread) { cout << "failed to create Thread" << endl; }
	else { CloseHandle(hThread); }

	int index = 0;
	while (1) {
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
		g_Clients[index].SetOnline(true);
		g_Clients[index].SetID(index);
		g_iCntClientNum++;
		
		DWORD ThreadId;
		hThread = CreateThread(NULL, 0, ProcessPacket, reinterpret_cast<LPVOID>(g_Clients[index].GetSockInfo()), 0, &ThreadId);

		if (NULL == hThread) { closesocket(g_Clients[index].GetSocket()); }
		else { CloseHandle(hThread); }
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
	cout << "id: " << session->id << endl;
	//When the players first come in, set stage 1
	g_Clients[session->id].SetStageNum(STAGE_1);
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
			break;
		}
		case CS_PLAYER_READY://
		{
			CS_PLAYER_READY_PACKET* packet = reinterpret_cast<CS_PLAYER_READY_PACKET*>(buf);
			if (packet->ready == READY_ON)
				g_Clients[sock_info->id].SetReady(true);
			else if(packet->ready == READY_OFF)
				g_Clients[sock_info->id].SetReady(false);
			break;
		}
		case CS_INPUT:
		{
			CS_INPUT_PACKET* packet = reinterpret_cast<CS_INPUT_PACKET*>(buf);
			//input key
			if (packet->state == KEY_PRESS)
			{
				g_Clients[session->id].SetKeyState(true);
				if (packet->type == KEY_FIREGUN)
				{
					if (g_Clients[session->id].GetGun())
					{
						//creat gun and shooting bullet
					}
				}
				else
					g_Clients[session->id].SetDirection(packet->key);
				g_Clients[sock_info->id].SetDirection(packet->key);
			}
			else if (packet->state == KEY_RELEASE)
			{
				g_Clients[session->id].SetKeyState(false);
				if (packet->type == KEY_FIREGUN);
				else
					g_Clients[session->id].SetDirection(0);
				g_Clients[sock_info->id].SetDirection(0);
			}
			break;
		}
		case CS_COLOR:
		{
			CS_PLAYER_COLOR_PACKET* packet = reinterpret_cast<CS_PLAYER_COLOR_PACKET*>(buf);
			packet_color = reinterpret_cast<PLAYER_COLOR*>(packet->color);
			//input color
			g_Clients[session->id].SetColor(*packet_color);
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
			switch (g_Clients[session->id].GetStageNum())
			{
			case STAGE_1:
				g_Clients[session->id].SetStageNum(STAGE_2);
				break;
			case STAGE_2:
				g_Clients[session->id].SetStageNum(STAGE_3);
				break;
			case STAGE_3:
				g_Clients[session->id].SetStageNum(STAGE_4);
				break;
			case STAGE_4:
				g_Clients[session->id].SetStageNum(STAGE_5);
				break;
			case STAGE_5:
				g_Clients[session->id].SetStageNum(STAGE_END);
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
