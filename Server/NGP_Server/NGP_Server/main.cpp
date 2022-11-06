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

//Client Update Thread
DWORD WINAPI ProcessPacket(LPVOID socket)
{
	return 0;
}

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