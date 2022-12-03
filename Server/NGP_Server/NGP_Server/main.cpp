#include "pch.h"
#include "TileMap.h"
#include "Bullet.h"
#include "Player.h"

//Global Variable
int g_iCntClientNum = 0;
array<Player, PLAYER_NUM> g_Clients;

array<unique_ptr<TileMap>, STAGE_NUM> g_TileMap;
unique_ptr<Bullet> g_Bullet = make_unique<Bullet>();

float g_ElapsedTime;

bool g_GameOver;
bool g_InGame = false;

bool g_bGun = false;
bool g_bCollideGun = false;
int g_iWhichStage = 0;
char g_chGStagePacket;
CRITICAL_SECTION g_CS;
CRITICAL_SECTION g_CS1;

void UploadMap();
void CreateGun();
void Collide_Bullet(Player* in, Bullet* Bullet);


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
		if (!g_InGame)
			continue;
		auto endTime = chrono::system_clock::now();
		g_ElapsedTime += chrono::duration<float, milli>(endTime - startTime).count();
		startTime = endTime;

		if ((g_ElapsedTime - 33.3f) >= DBL_EPSILON) {
			if (g_iCntClientNum > 0) {
				if (g_Clients[0].GetOnline()) {
					packet->dir_p1 = g_Clients[0].GetDirection();
					packet->stage_p1 = g_Clients[0].GetStageNum();
					packet->x_p1 = g_Clients[0].GetPos().x;
					packet->y_p1 = g_Clients[0].GetPos().y;
				}
				if (g_Clients[1].GetOnline()) {
					packet->dir_p2 = g_Clients[1].GetDirection();
					packet->stage_p2 = g_Clients[1].GetStageNum();
					packet->x_p2 = g_Clients[1].GetPos().x;
					packet->y_p2 = g_Clients[1].GetPos().y;
				}
				if (g_Clients[2].GetOnline()) {					
					packet->dir_p3 = g_Clients[2].GetDirection();
					packet->stage_p3 = g_Clients[2].GetStageNum();
					packet->x_p3 = g_Clients[2].GetPos().x;
					packet->y_p3 = g_Clients[2].GetPos().y;
				}
				if (g_Bullet->GetbShow()) {
					packet->bullet_enable = BULLET_ON;
					packet->dir_bullet = g_Bullet->GetDirection();
					packet->stage_bullet = g_Bullet->GetStage();
					packet->x_bullet = g_Bullet->GetPos().x;
					packet->y_bullet = g_Bullet->GetPos().y;
				}
				else {
					if (g_bGun)
					{
						packet->bullet_enable = GUN_OBJECT;
						packet->dir_bullet = 0;
						switch (g_iWhichStage)
						{
						case 0:
							packet->stage_bullet = STAGE_1;
							break;
						case 1:
							packet->stage_bullet = STAGE_2;
							break;
						case 2:
							packet->stage_bullet = STAGE_3;
							break;
						case 3:
							packet->stage_bullet = STAGE_4;
							break;
						case 4:
							packet->stage_bullet = STAGE_5;
							break;
						default:
							cout << "stage End or bug" << endl;
							break;
						}
						packet->x_bullet = g_TileMap[g_iWhichStage].get()->GetGunPos().x;
						packet->y_bullet = g_TileMap[g_iWhichStage].get()->GetGunPos().y;
					}
					else
						packet->bullet_enable = BULLET_OFF;
				}
				for (int i = 0; i < PLAYER_NUM; ++i) {
					if (g_Clients[i].GetOnline()) {
						EnterCriticalSection(&g_CS);
						send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(&update_len), sizeof(int), 0);
						send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(packet), update_len, 0);
						LeaveCriticalSection(&g_CS);
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
	InitializeCriticalSection(&g_CS);
	InitializeCriticalSection(&g_CS1);
	float MilliTime = 0.f;
	float ElapsedTime = 0.f;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		cout << "socket error" << endl;

	int opt = 1;
	setsockopt(listen_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt, sizeof(int));

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

	// Login
	int index = 0;
	UploadMap();
	while (!g_InGame) {
		if (g_iCntClientNum > 3)
			continue;
		else {
			for (int i = 0; i < PLAYER_NUM; ++i) {
				if (!g_Clients[i].GetAccessUser()) {
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
		g_Clients[index].SetAccessUser(true);
		g_iCntClientNum++;

		DWORD ThreadId;
		hThread = CreateThread(NULL, 0, ProcessPacket, reinterpret_cast<LPVOID>(g_Clients[index].GetSockInfo()), 0, &ThreadId);

		if (NULL == hThread) { closesocket(g_Clients[index].GetSocket()); }
		else { CloseHandle(hThread); }

		if (g_iCntClientNum > 2) {
			break;
		}
	}

	//Lobby
	int cnt = 0;
	while (true) {
		for (int i = 0; i < 3; ++i) {
			if (!g_Clients[i].GetReady())
			{
				cnt = 0;
				break;
			}
			else
				cnt++;
		}
		if (cnt == 3) {
			SC_GAMESTART_PACKET* packet = new SC_GAMESTART_PACKET;
			packet->type = SC_GAMESTART;
			int len = sizeof(SC_GAMESTART_PACKET);
			for (int i = 0; i < 3; ++i) {
				EnterCriticalSection(&g_CS);
				send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
				send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
				LeaveCriticalSection(&g_CS);
			}
			g_InGame = true;
			cout << "InGame" << endl;
			break;
		}
	}

	// Main Loop
	auto endTime = chrono::steady_clock::now();
	auto StartT = endTime;
	float GunCoolTime = chrono::duration_cast<chrono::milliseconds>(endTime - StartT).count();
	CreateGun();
	while (true) {
		endTime = chrono::steady_clock::now();
		MilliTime = chrono::duration_cast<chrono::milliseconds>(endTime - StartT).count();
		ElapsedTime = MilliTime / 1000.f;
		if (ElapsedTime >= (1.f / 60.f)) {
			//InGame
			// Bullet, Move, Collide
			if (g_Bullet->GetbShow()) {
				g_Bullet->Update(ElapsedTime);
				for (int i = 0; i < PLAYER_NUM; ++i)
				{
					Collide_Bullet(&g_Clients[i], g_Bullet.get());
				}
			}

			// Gun CoolTime
			GunCoolTime += ElapsedTime;
			if (GunCoolTime >= 10.f)
			{
				//into zero, collision zero
				g_bGun = false;
				if (GunCoolTime >= 11.f)
				{
					GunCoolTime = 0.f;
					CreateGun();
				}
			}


			// Player, Move, Collides

			for (int i = 0; i < PLAYER_NUM; ++i) {
				g_Clients[i].Update(ElapsedTime);
			}

			for (int i = 0; i < PLAYER_NUM; ++i) 
				if (g_TileMap[g_Clients[i].GetStageNum() - 1] && g_Clients[i].GetStageNum() < STAGE_END)
				{
					g_TileMap[g_Clients[i].GetStageNum() - 1]->Collide_Gate(&g_Clients[i]);
					g_TileMap[g_Clients[i].GetStageNum() - 1]->Collide_Jump(&g_Clients[i]);
					g_TileMap[g_Clients[i].GetStageNum() - 1]->Collide_Wall(&g_Clients[i]);
					if (!g_Clients[i].GetGun())
					{
						g_Clients[i].SetGun(g_TileMap[g_Clients[i].GetStageNum() - 1]->Collide_Gun(&g_Clients[i]));
					}
					else
					{
						g_bGun = false;
						g_bCollideGun = true;
					}
						
				}
					
			if (g_bCollideGun)
			{
				GunCoolTime = 0.f;
			}

			StartT = endTime;
		}
		else {
			Sleep(1);
		}
	}

	closesocket(listen_sock);

	WSACleanup();

	DeleteCriticalSection(&g_CS);
	DeleteCriticalSection(&g_CS1);

	return 0;
}

void UploadMap()
{
	g_TileMap[0] = make_unique<TileMap>("Server_File\\Stage1.txt");
	g_TileMap[1] = make_unique<TileMap>("Server_File\\Stage2.txt");
	g_TileMap[2] = make_unique<TileMap>("Server_File\\Stage3.txt");
	g_TileMap[3] = make_unique<TileMap>("Server_File\\Stage4.txt");
	g_TileMap[4] = make_unique<TileMap>("Server_File\\Stage5.txt");
	// 0: Nothing,      D: wall
	// Z: Red Jump,     Y: Green Jump,     X: Blue Jump,     W: Yellow Jump  V: Purple Jump		U: GB Jump
	// !: Red Gate		@: Green Gate,     #: Blue Gate,	 $: Yellow Gate  %: Purple Gate		^: GB Gate
	// A: Gun			S: Gun possible space
}

int median(int a, int b, int c)
{
	int maxium = max(max(a, b), c);
	int mininum = min(min(a, b), c);
	return a ^ b ^ c ^ maxium ^ mininum;
}

void CreateGun()
{
	g_iWhichStage = 0;
	array<int, 3> PlayerStage;
	for (int i = 0; i < PLAYER_NUM; ++i)
	{
		PlayerStage[i] = g_Clients[i].GetStageNum();
	}
	g_iWhichStage = median(PlayerStage[0], PlayerStage[1], PlayerStage[2]);
	g_iWhichStage -= 1;
	cout << "CreatGun stage: "<< g_iWhichStage << endl;
	g_TileMap[g_iWhichStage]->CreateGun();
	g_bGun = true;
	//After CreateGun, SendPacket

}

DWORD WINAPI ProcessPacket(LPVOID socket)
{
	int retval;
	SOCK_INFO* sock_info = reinterpret_cast<SOCK_INFO*> (socket);

	SOCKET client_sock = sock_info->client_sock;
	int len;
	char buf[BUF_SIZE];
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
		case CS_LOGIN://여기서 ready??
		{
			CS_LOGIN_PACKET* packet = reinterpret_cast<CS_LOGIN_PACKET*>(buf);
			g_Clients[sock_info->id].SetName(packet->name);
			g_Clients[sock_info->id].SetOnline(true);
			cout << "이름: " << g_Clients[sock_info->id].GetName() << endl;
			// Send Login Info packet to Client which logined
			SC_LOGIN_INFO_PACKET* scp = new SC_LOGIN_INFO_PACKET;
			scp->type = SC_LOGIN_INFO;
			
			scp->online_p1 = g_Clients[0].GetOnline();
			scp->online_p2 = g_Clients[1].GetOnline();
			scp->online_p3 = g_Clients[2].GetOnline();
			scp->ready_p1 = g_Clients[0].GetReady();
			scp->ready_p2 = g_Clients[1].GetReady();
			scp->ready_p3 = g_Clients[2].GetReady();
			memcpy(scp->name_p1, g_Clients[0].GetName(), NAME_SIZE);
			memcpy(scp->name_p2, g_Clients[1].GetName(), NAME_SIZE);
			memcpy(scp->name_p3, g_Clients[2].GetName(), NAME_SIZE);
			len = sizeof(SC_LOGIN_INFO_PACKET);
			for (int i = 0; i < PLAYER_NUM; ++i) {
				if (i == sock_info->id)
					scp->id = sock_info->id;
				else
					scp->id = -1;
				if (g_Clients[i].GetOnline()) {					
					EnterCriticalSection(&g_CS);
					send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(scp), len, 0);
					LeaveCriticalSection(&g_CS);
				}
			}
			delete scp;

			cout << "Login" << endl;
			break;
		}
		case CS_PLAYER_READY:
		{
			CS_PLAYER_READY_PACKET* packet = reinterpret_cast<CS_PLAYER_READY_PACKET*>(buf);
			if (packet->ready == READY_ON) {
				g_Clients[sock_info->id].SetReady(true);

				// Send Ready packet to Clients
				SC_READY_PACKET* scp = new SC_READY_PACKET;
				scp->type = SC_READY;
				scp->ready = READY_ON;
				scp->id = sock_info->id;
				len = sizeof(SC_READY_PACKET);
				for (int i = 0; i < 3; ++i) {
					EnterCriticalSection(&g_CS);
					send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(scp), len, 0);
					LeaveCriticalSection(&g_CS);
				}
				delete scp;
			}
			else if (packet->ready == READY_OFF) {
				g_Clients[sock_info->id].SetReady(false);

				// Send Ready packet to Clients
				SC_READY_PACKET* scp = new SC_READY_PACKET;
				scp->type = SC_READY;
				scp->ready = READY_OFF;
				scp->id = sock_info->id;
				len = sizeof(SC_READY_PACKET);
				for (int i = 0; i < 3; ++i) {
					EnterCriticalSection(&g_CS);
					send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(scp), len, 0);
					LeaveCriticalSection(&g_CS);
				}
				delete scp;
			}

			break;
		}
		case CS_INPUT:
		{
			CS_INPUT_PACKET* packet = reinterpret_cast<CS_INPUT_PACKET*>(buf);
			//input key
			if (packet->state == KEY_PRESS)
			{
				//std::cout << sock_info->id << "-" << (int)packet->key << "키를 누름" << std::endl;
				g_Clients[sock_info->id].SetKeyState(true);
				if (packet->key == KEY_FIREGUN)
				{
					if (g_Clients[sock_info->id].GetGun())
					{
						//shooting bullet
						if (!g_Bullet->GetbShow())
						{
							g_Bullet->SetbShow(true);
							g_Bullet->SetDirection(g_Clients[sock_info->id].GetSubDirection());
							g_Bullet->SetStage(g_Clients[sock_info->id].GetStageNum());
							Vec2 pos = g_Clients[sock_info->id].GetPos();
							pos.x += g_Bullet->GetDirection() * TILE_SIZE * 1.0f;
							g_Bullet->SetPos(pos);
						}
					}
				}
				else {
					switch (packet->key) {
					case KEY_DIR_LEFT:
						g_Clients[sock_info->id].SetDirection(LEFT);
						g_Clients[sock_info->id].SetSubDirection(LEFT);
						break;
					case KEY_DIR_RIGHT:
						g_Clients[sock_info->id].SetDirection(RIGHT);
						g_Clients[sock_info->id].SetSubDirection(RIGHT);
						break;
					case KEY_DIR_UP:
						cout << g_Clients[sock_info->id].GetJump();
						if (!g_Clients[sock_info->id].GetJump())
						{
							g_Clients[sock_info->id].SetJumpCount(0);
							g_Clients[sock_info->id].SetJump(true);
						}
						break;
					default:
						break;
					}
				}
			}
			else if (packet->state == KEY_RELEASE)
			{
				//std::cout << sock_info->id << "-" << (int)packet->key << "키를 뗌" << std::endl;
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
			//input color
			g_Clients[sock_info->id].SetColor(static_cast<PLAYER_COLOR>(packet->color));
			for (int i = 0; i < PLAYER_NUM; ++i) {
				if (sock_info->id == i)
					continue;
				SC_COLOR_PACKET* scp = new SC_COLOR_PACKET;
				scp->type = SC_COLOR;
				scp->id = sock_info->id;
				scp->color = packet->color;
				len = sizeof(SC_COLOR_PACKET);

				EnterCriticalSection(&g_CS);
				send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
				send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(scp), len, 0);
				LeaveCriticalSection(&g_CS);
				delete scp;
			}

			break;
		}
		case CS_PLAYER_RESET:
		{
			CS_PLAYER_RESET_PACKET* packet = reinterpret_cast<CS_PLAYER_RESET_PACKET*>(buf);
			if (packet->reset == RESET_ON)
			{
				//player go back to original position
				g_Clients[sock_info->id].SetPos(Vec2{ TILE_SIZE, WINDOW_HEIGHT - 2 * TILE_SIZE });
				//set original color
				g_Clients[sock_info->id].SetColor(PLAYER_COLOR::NORMAL);
				g_Clients[sock_info->id].SetDirection(0);
				g_Clients[sock_info->id].SetGun(false);
			}
			break;
		}
		case CS_NEXTSTAGE:
		{
			switch (g_Clients[sock_info->id].GetStageNum())
			{
			case STAGE_1:
				g_Clients[sock_info->id].SetStageNum(STAGE_2);
				g_Clients[sock_info->id].SetPos(Vec2{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
				g_Clients[sock_info->id].SetFall(false);
				break;
			case STAGE_2:
				g_Clients[sock_info->id].SetStageNum(STAGE_3);
				g_Clients[sock_info->id].SetPos(Vec2{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
				g_Clients[sock_info->id].SetFall(false);
				break;
			case STAGE_3:
				g_Clients[sock_info->id].SetStageNum(STAGE_4);
				g_Clients[sock_info->id].SetPos(Vec2{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
				g_Clients[sock_info->id].SetFall(false);
				break;
			case STAGE_4:
				g_Clients[sock_info->id].SetStageNum(STAGE_5);
				g_Clients[sock_info->id].SetPos(Vec2{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
				g_Clients[sock_info->id].SetFall(false);
				break;
			case STAGE_5:
			{
				for(int i = 0; i < PLAYER_NUM; ++i)
					g_Clients[i].SetStageNum(STAGE_END);
				//send rank all player
				SC_RANK_PACKET* packet = new SC_RANK_PACKET;
				packet->type = SC_RANK;
				memcpy(packet->winner_name, g_Clients[sock_info->id].GetName(), NAME_SIZE);
				len = sizeof(SC_RANK_PACKET);
				for (int i = 0; i < PLAYER_NUM; ++i) {
					if (i != sock_info->id)
					{
						EnterCriticalSection(&g_CS);
						send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(len), 0);
						send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
						LeaveCriticalSection(&g_CS);
					}
				}
				delete packet;
			}
			break;
			}
		default:
			break;
		}
		break;
		}
	}
	closesocket(client_sock);
	return 0;
}

void Collide_Bullet(Player* in, Bullet* Bullet)
{
	//int bulletStage = 0;
	//switch (Bullet->GetStage())
	//{
	//case STAGE_1:
	//	bulletStage = 1;
	//	break;
	//case STAGE_2:
	//	bulletStage = 2;
	//	break;
	//case STAGE_3:
	//	bulletStage = 3;
	//	break;
	//case STAGE_4:
	//	bulletStage = 4;
	//	break;
	//case STAGE_5:
	//	bulletStage = 5;
	//	break;
	//case STAGE_END:
	//	break;
	//default:
	//	break;
	//}

	//if (in->GetStageNum() == bulletStage)
	if (in->GetStageNum() == Bullet->GetStage())
	{
		Vec2 BulletPos = Bullet->GetPos();
		Vec2 inLT = { in->GetPos().x + TILE_SIZE / 4 + 3, in->GetPos().y + 3 };
		Vec2 inRB = { in->GetPos().x + TILE_SIZE * 0.75f - 3, in->GetPos().y + TILE_SIZE - 3 };

		bool bCollide = false;

		Vec2 bulletLT = { BulletPos.x + TILE_SIZE / 4 + 3, BulletPos.y + TILE_SIZE / 4 + 3 };
		Vec2 bulletRB = { BulletPos.x + TILE_SIZE * 0.75f - 3, BulletPos.y + TILE_SIZE * 0.75f - 3 };

		if (inLT.x > bulletRB.x);
		else if (inRB.x < bulletLT.x);
		else if (inLT.y > bulletRB.y);
		else if (inRB.y < bulletLT.y);
		else {
			g_Bullet->SetbShow(false);
			g_bCollideGun = false;
			in->SetPos(Vec2{ TILE_SIZE, WINDOW_HEIGHT - 2 * TILE_SIZE });
			in->SetColor(PLAYER_COLOR::NORMAL);
			in->SetDirection(0);

			SC_RESET_PACKET* packet = new SC_RESET_PACKET;
			packet->type = SC_RESET;
			packet->id = in->GetID();
			int len = sizeof(SC_RESET_PACKET);
			for (int i = 0; i < 3; ++i) {
				EnterCriticalSection(&g_CS);
				send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
				send(g_Clients[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
				LeaveCriticalSection(&g_CS);
			}
			delete packet;

			for (int i = 0; i < PLAYER_NUM; ++i)
			{
				g_Clients[i].SetGun(false);
			}

		}
	}
	if (Bullet->GetPos().x<0 - TILE_SIZE || Bullet->GetPos().x > WINDOW_WIDTH + TILE_SIZE)
	{
		g_Bullet->SetbShow(false);
		g_bCollideGun = false;
		for (int i = 0; i < PLAYER_NUM; ++i)
		{
			g_Clients[i].SetGun(false);
		}
	}
	
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
