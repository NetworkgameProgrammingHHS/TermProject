#include "pch.h"
#include "TileMap.h"
#include "Bullet.h"
#include "Player.h"

//Global Variable
array<Player, PLAYER_NUM> g_Clients;
array<unique_ptr<TileMap>, STAGE_NUM> g_TileMap;
unique_ptr<Bullet> g_Bullet;
bool g_GameOver;
CRITICAL_SECTION g_CS;
float g_ElapsedTime;
int g_iCntClientNum = 0;

//Client Update Thread
DWORD WINAPI ProcessPacket(LPVOID socket);
//if complete, delete
void err_display(const char* msg);
void err_display(int errcode);

// Packet Send Thread
DWORD WINAPI SendPacket(LPVOID)
{
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

	SESSION* client_session = new SESSION;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, SendPacket, NULL, 0, 0);
	if (NULL == hThread) { cout << "failed to create Thread" << endl; }
	else { CloseHandle(hThread); }

	while (1) {
		addrlen = sizeof(clientaddr);
		client_session->client_sock = accept(listen_sock, reinterpret_cast<struct sockaddr*>(&clientaddr), &addrlen);
		if (client_session->client_sock == INVALID_SOCKET) {
			cout << "accept error" << endl;
			exit(1);
		}
		//set client id
		client_session->id = g_iCntClientNum++;
		DWORD ThreadId;
		hThread = CreateThread(NULL, 0, ProcessPacket, reinterpret_cast<LPVOID>(client_session), 0, &ThreadId);

		if (NULL == hThread) { closesocket(client_session->client_sock); }
		else { CloseHandle(hThread); }
	}

	closesocket(listen_sock);

	WSACleanup();

	delete client_session;

	return 0;
}

DWORD WINAPI ProcessPacket(LPVOID socket)
{
	int retval;
	SESSION* session = reinterpret_cast<SESSION*> (socket);

	SOCKET client_sock = session->client_sock;
	int len;
	char buf[BUF_SIZE];
	cout << "id: " << session->id << endl;
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
		case CS_LOGIN:
		{
			CS_LOGIN_PACKET* packet = reinterpret_cast<CS_LOGIN_PACKET*>(buf);
			g_Clients[session->id].SetName(packet->name);
			break;
		}
		case CS_PLAYER_READY:
		{
			CS_PLAYER_READY_PACKET* packet = reinterpret_cast<CS_PLAYER_READY_PACKET*>(buf);
			break;
		}
		case CS_INPUT:
		{
			CS_INPUT_PACKET* packet = reinterpret_cast<CS_INPUT_PACKET*>(buf);
			break;
		}
		case CS_COLOR:
		{
			CS_PLAYER_COLOR_PACKET* packet = reinterpret_cast<CS_PLAYER_COLOR_PACKET*>(buf);
			break;
		}
		case CS_PLAYER_RESET:
		{
			CS_PLAYER_RESET_PACKET* packet = reinterpret_cast<CS_PLAYER_RESET_PACKET*>(buf);
			break;
		}
		case CS_GAMECLEAR:
		{
			CS_GAMECLEAR_PACKET* packet = reinterpret_cast<CS_GAMECLEAR_PACKET*>(buf);
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
